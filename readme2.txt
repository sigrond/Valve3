Kroki niezbêdne do przekompilowania kodu:

projekt zaimportowaæ

zainstalowaæ Microsoft Windows SDK for Windows 7 and .NET Framework 4

http://stackoverflow.com/questions/3481713/how-to-get-started-with-directshow

skompilowaæ i zlinkowaæ DirectShow baseclasses

ustawiæ odpowiednie œcie¿ki w projekcie

https://social.msdn.microsoft.com/forums/windowsdesktop/en-us/5da8f0b8-d2a9-4caf-81e1-7f5788fa1c00/base-classes-compile-problem-with-vista-sdk

include baseclasses musi byæ przed sdk

http://stackoverflow.com/questions/29717190/how-to-make-ws2-32-lib-work-with-vs-2013-project-targeted-at-windows-8-1

ustawiæ linkowanie ws2_32.lib


¿eby linkowa³o siê poprwanie w visual studio 2013 doda³em do kodu kilka dyrektyw preprocesora


napisa³em plik wsadowy set_up_filter.bat, który nale¿y uruchomiæ jako administrator.
skopiuje on filtr do system32 i zatrejestruje

teraz w ustawieniach projektu w post operacjach wy³¹czy³em bezmyœln¹ próbê zarejestrowania filtru i nie sypie b³êdami


od teraz serwer uruchamia siê dopiero przy rozpoczêciu transformacji,
¿eby mo¿na by³o przed uruchomieniem ustawiæ we w³aœciwoœciach inny port tcp/ip


repozytorium: https://github.com/sigrond/Valve3

linkowa³a siê z³a wersja strmbase.lib
podmieni³em w D:\Program Files\Windows Kits\8.1\Lib\winv6.3\um\x86 na moj¹ wersjê