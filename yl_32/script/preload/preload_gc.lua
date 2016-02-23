--固定预先加载的脚本，会是所有脚本加载的第一个

if not MODULE_GC_SERVER then
	return
end

print("Preload gc script files...");

