
PTROcxps.dll: dlldata.obj PTROcx_p.obj PTROcx_i.obj
	link /dll /out:PTROcxps.dll /def:PTROcxps.def /entry:DllMain dlldata.obj PTROcx_p.obj PTROcx_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PTROcxps.dll
	@del PTROcxps.lib
	@del PTROcxps.exp
	@del dlldata.obj
	@del PTROcx_p.obj
	@del PTROcx_i.obj
