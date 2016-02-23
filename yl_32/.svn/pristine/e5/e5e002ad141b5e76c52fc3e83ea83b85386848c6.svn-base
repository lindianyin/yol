/**
 * Profiler for Lua 5.1 alpha.
 * Copyright (C) 2005, 2007 Wolfgang Oertl
 *
 * Usage:
 *
 * require "profiler"
 * profiler.start()
 *
 * ...
 *
 * profiler.stop()
 * profiler.dump()
 *
 *
 * During execution no output is generated, so the overhead is very small.  The
 * resulting listing of functions has the following columns:
 *
 * - number of times the function has been called
 * - runtime of the function excluding subroutine invocations
 * - number of subroutine invocations
 * - runtime of subroutines
 *
 * Recursive functions get the time counted only once (as runtime).  As a
 * timing source, clock_gettime() is used on Linux and GetSystemTimeAsFileTime
 * on Windows.
 *
 * TODO
 * - the profiler doesn't try to subtract its own time from the runtime.
 * - it might be feasible to store a call graph.
 */


#include "stdafx.h"
//#include <lua.h>
//#include <lauxlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
};


#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#define snprintf _snprintf
#define INLINE __inline
#else
#define INLINE inline
#endif

static int profiler_running = 0;

// used as "user data" in the function table
struct prof_func {
	int		call_counter;
	int		callout_counter;	// number of subroutines called
	int		currently_running;	// is it in the call stack?
	int		total_time;		// µs spent in this function
	int		subroutine_time;	// µs spent in subroutines
};


// used on the private call stack
struct prof_item {
	struct prof_item *prev, *next;
	int		start_total;		// start of invocation
	int		start_callout;		// start of last subfunc call
	struct prof_func *func_info;
};
static struct prof_item *prof_stack_top, stack_sentinel;

static int stack_level = 1;	/* current depth of stack -- for debugging */
static int ignore_leave;	/* first leave call from prof_start */


/**
 * When reaching a stack depth never reached before, allocate a new item.
 */
static void prof_stack_allocate()
{
	struct prof_item *item = (struct prof_item*) malloc(sizeof *item);
	item->next = NULL;
	item->prev = prof_stack_top;
	prof_stack_top->next = item;
#ifdef TRACE
	printf("profiler: alloc stack item at level %d\n", stack_level);
#endif
}

#ifdef DEBUG
static void prof_stack_check()
{
	struct prof_item *item = prof_stack_top;
	int count = 0;

	while (item) {
		count ++;
		if (item->prev && item != item->prev->next)
			printf("ERROR in chaining (1)\n");
		item = item->prev;
		if (item && item->next->prev != item)
			printf("ERROR in chaining (2)\n");
	}
	if (count != stack_level)
		printf("ERROR in stack level: %d - %d\n", count, stack_level);
}
#else
#define prof_stack_check()
#endif

/**
 * Puts a new (or reused) item onto the stack and returns it
 */
static INLINE struct prof_item *prof_stack_push()
{
	if (!prof_stack_top->next)
		prof_stack_allocate();

	prof_stack_top = prof_stack_top->next;
	stack_level ++;
	prof_stack_check();
	return prof_stack_top;
}

/**
 * Retrieves the topmost item of the stack, and moves the stack top down.
 * Note that the item is not deallocated, but can be reused later.
 */
static INLINE struct prof_item *prof_stack_pop()
{
	struct prof_item *item = prof_stack_top;
	if (item)
		prof_stack_top = item->prev;
	stack_level --;
	prof_stack_check();
	return item;
}

/**
 * Deallocate the whole stack above the current stack top.
 * This is a somewhat superfluous routine.
 */
static void prof_stack_free()
{
	struct prof_item *item_next;
	int count = 0;

	while (prof_stack_top->next) {
		item_next = prof_stack_top->next->next;
		free(prof_stack_top->next);
		prof_stack_top->next = item_next;
		count = count + 1;
	}
	prof_stack_top = NULL;
}

static void prof_enter(lua_State *L, int now, lua_Debug *ar);
static void prof_leave(lua_State *L, int now);


/**
 * Get current timestamp as microseconds
 */
#ifdef WIN32
static ULARGE_INTEGER base_time, now, get_time_tmp;
static INLINE int get_time()
{
	FILETIME t;
	GetSystemTimeAsFileTime(&t);
	now.LowPart = t.dwLowDateTime;
	now.HighPart = t.dwHighDateTime;
	get_time_tmp.QuadPart = now.QuadPart - base_time.QuadPart;
	return (int) ((get_time_tmp.QuadPart) / 10);
}

static void get_time_init()
{
	FILETIME t;
	GetSystemTimeAsFileTime(&t);
	base_time.LowPart = t.dwLowDateTime;
	base_time.HighPart = t.dwHighDateTime;
}

#else
static struct timespec res;
static int base_time;		// in seconds
static INLINE int get_time()
{
	clock_gettime(CLOCK_REALTIME, &res);
	return (res.tv_sec - base_time) * 1000 * 1000 + res.tv_nsec / 1000;
}

static void get_time_init()
{
	clock_gettime(CLOCK_REALTIME, &res);
	base_time = res.tv_sec;
}

#endif


/**
 * Profiler function, called on function entry and exit.
 */
static void prof_hook(lua_State *L, lua_Debug *ar)
{
	int now = get_time();

	lua_getfield(L, LUA_REGISTRYINDEX, "prof_table");
#ifdef DEBUG
	if (lua_isnil(L, -1)) {
		printf("ERROR: prof_table is not registered.\n");
		return;
	}
#endif

	switch (ar->event) {
		case LUA_HOOKCALL:
			prof_enter(L, now, ar);
			break;

		case LUA_HOOKRET:
		case LUA_HOOKTAILRET:
			prof_leave(L, now);
			break;
		default:
			printf("ERROR: invalid profiler event %d\n", ar->event);
	}
}

static void prof_enter(lua_State *L, int now, lua_Debug *ar)
{
	int len;
	char buf[1024];
	struct prof_func *func_info;

	/* identify the function */
	lua_getinfo(L, "nS", ar);

	/* make a key out of that */
	len = snprintf(buf, sizeof(buf), "%s@%d:%s",
		ar->source, ar->linedefined, ar->name);

	/* look it up */
	lua_pushlstring(L, buf, len);
	lua_pushvalue(L, -1);
	lua_rawget(L, -3);
	if (lua_isnil(L, -1)) {
		/* first call to the function */
		lua_pop(L, 1);
		func_info = (struct prof_func*) lua_newuserdata(L,
			sizeof(*func_info));
		memset(func_info, 0, sizeof(*func_info));
		lua_rawset(L, -3);
	} else {
		func_info = (struct prof_func*) lua_touserdata(L, -1);
		lua_pop(L, 2);
	}

	func_info->call_counter ++;

	/* mark previously running function as having called a subfunction */
	prof_stack_top->start_callout = now;
	prof_stack_top->func_info->callout_counter ++;

	/* func_info is set up, push local stack */
	struct prof_item *item = prof_stack_push();

	item->func_info = func_info;
	item->start_total = now;
	item->start_callout = -1;

	/* detect recursion */
	func_info->currently_running++;

}

static void prof_leave(lua_State *L, int now)
{
	/* first leave after the profiler started -- ignore */
	if (!prof_stack_top->prev) {
		if (!ignore_leave)
			printf("WARNING: prof_leave at stack base, ignored\n");
		ignore_leave = 0;
		return;
	}

	struct prof_item *item = prof_stack_pop();

#ifdef DEBUG
	if (!item) {
		printf("ERROR: stack is empty on prof_leave.\n");
		return;
	}

	if (!item->prev) {
		printf("ERROR: stack sentinel popped!\n");
		return;
	}

	if (item->start_callout >= 0) {
		printf("ERROR: stack top has a callout set.\n");
	}
#endif

	/* If this function invoked itself, do not count this as subroutine
	 * time, because the the time would be counted double */
	if (-- item->func_info->currently_running) {
		item->func_info->subroutine_time -= (now - item->start_total);
	} else {
		/* add to total runtime of function (not in recursion) */
		item->func_info->total_time += now - item->start_total;
	}

	/* adjust subroutine time of callee */
	prof_stack_top->func_info->subroutine_time +=
		now - prof_stack_top->start_callout;
	prof_stack_top->start_callout = -1;
}


/**
 * Start profiling; this means to set up hooks
 */
int prof_start(lua_State *L)
{
	struct prof_func *fi;

	if (profiler_running++ > 0)
		return 0;

	lua_sethook(L, prof_hook, LUA_MASKCALL | LUA_MASKRET, 0);

	/* manually set up the stack with one base entry */
	fi = stack_sentinel.func_info = (struct prof_func*) malloc(sizeof
		(struct prof_func));
	memset(fi, 0, sizeof(*fi));
	prof_stack_top = &stack_sentinel;
	prof_stack_top->prev = NULL;
	stack_level = 1;
	ignore_leave = 1;

	/* all stored times are relative to the start time to avoid having
	 * to use very large integers or floating point numbers */
	get_time_init();

	return 0;
}


/**
 * Stop profiling.
 */
int prof_stop(lua_State *L)
{
	if (--profiler_running > 0)
		return 0;

	/* One item should be on the stack: the call to this function. */
	if (prof_stack_top->prev && prof_stack_top->prev->prev) {
		printf("WARNING: profiler stopped at a different "
			"stack level.\n");
	}

	int now = get_time();

	while (prof_stack_top->prev)
		prof_leave(L, now);

	prof_stack_free();
	free(stack_sentinel.func_info);
	stack_sentinel.func_info = NULL;
#ifdef DEBUG
	if (prof_stack_top->next) {
		printf("WARNING: profiler stack not empty after stop()\n");
	}
#endif

	lua_sethook(L, NULL, 0, 0);

	return 0;
}


int prof_dump_buffer(lua_State *L, std::stringstream& stmBuffer)
{
	struct prof_func *func_info;
	int total_time = 0;

	while (profiler_running > 0)
		prof_stop(L);

	/* header */
	//printf("%8s  %11s  %8s  %11s %s\n",
	//	"calls", "own time", "subs", "sub time", "function");
	//printf("-------------------------------------\n");
	stmBuffer << "calls\t" << "time\t" << "self time\t" << "subs\t" << "sub time\t" << "function\n";
	stmBuffer << "-------------------------------------------------\n";

	/* show all entries */
	lua_getfield(L, LUA_REGISTRYINDEX, "prof_table");
	int prof_table = lua_gettop(L);
	lua_pushnil(L);

	while (lua_next(L, prof_table)) {
		func_info = (prof_func*)lua_touserdata(L, -1);

		//printf("%7d  %10.3f  %7d  %10.3f %.*s\n",
		//	func_info->call_counter,
		//	(func_info->total_time - func_info->subroutine_time)
		//	/ 1000.0 / 1000,
		//	func_info->callout_counter,
		//	func_info->subroutine_time / 1000.0 / 1000,
		//	lua_objlen(L, -2),
		//	lua_tostring(L, -2));

		stmBuffer << func_info->call_counter << "\t"
			<< func_info->total_time / 1000.0f / 1000 << "\t"
			<< (func_info->total_time - func_info->subroutine_time) / 1000.0f / 1000 << "\t"
			<< func_info->callout_counter << "\t"
			<< func_info->subroutine_time / 1000.0f / 1000 << "\t"
			<< lua_tostring(L, -2) << "\n";

		total_time += func_info->total_time - func_info->subroutine_time;
		lua_pop(L, 1);
	}

	//printf("-------------------------------------\n");
	//printf("  Total:  %10.3f\n", total_time / 1000.0 / 1000);
	stmBuffer << "-------------------------------------\n";
	stmBuffer << "Total: " << total_time / 1000.0f / 1000 << "\n";

	return 0;
}

/**
 * Write the results to a file (or rather, stdout).
 */
int prof_dump(lua_State *L)
{
	std::stringstream stmBuffer;
	prof_dump_buffer(L, stmBuffer);
	printf("%s", stmBuffer.str().c_str());
	return 0;
}

static const luaL_reg lib_methods[] = {
	{ "start", prof_start },
	{ "stop", prof_stop },
	{ "dump", prof_dump },
	{ NULL, NULL }
};


int luaopen_profiler(lua_State *L)
{
	//luaL_register(L, "profiler", lib_methods);
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, "prof_table");
	return 1;
}
