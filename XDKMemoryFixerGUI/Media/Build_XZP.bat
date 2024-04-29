@if exist XDKMF.xzp del XDKMF.xzp

@pushd xzp_contents

@set XUIPKG="%XEDK%\bin\win32\xuipkg.exe" /nologo

@set XUI_XZP="%~dp0%XDKMF.xzp"
@set XUI_FILES=*.*

@echo Building XDKMF.xzp

%XUIPKG% /R /O %XUI_XZP% %XUI_FILES%

@popd

@cmd /k