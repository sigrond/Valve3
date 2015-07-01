echo BEGIN DRAG-AND-DROP %n1 REGISTRAR FOR 32-BIT SYSTEMS
copy %1 %windir%\System32
regsvr32 "%nx1"
echo END BATCH FILE
pause