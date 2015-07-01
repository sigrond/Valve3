echo BEGIN REGISTRAR FOR 32-BIT SYSTEMS
@setlocal enableextensions @cd /d "%~dp0"
@cd /d "%~dp0"
copy Valve_echo2.ax %windir%\System32
regsvr32 "Valve_echo2.ax"
echo END BATCH FILE
pause