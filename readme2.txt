Kroki niezb�dne do przekompilowania kodu:

projekt zaimportowa�

zainstalowa� Microsoft Windows SDK for Windows 7 and .NET Framework 4

http://stackoverflow.com/questions/3481713/how-to-get-started-with-directshow

skompilowa� i zlinkowa� DirectShow baseclasses

ustawi� odpowiednie �cie�ki w projekcie

https://social.msdn.microsoft.com/forums/windowsdesktop/en-us/5da8f0b8-d2a9-4caf-81e1-7f5788fa1c00/base-classes-compile-problem-with-vista-sdk

include baseclasses musi by� przed sdk

http://stackoverflow.com/questions/29717190/how-to-make-ws2-32-lib-work-with-vs-2013-project-targeted-at-windows-8-1

ustawi� linkowanie ws2_32.lib


�eby linkowa�o si� poprwanie w visual studio 2013 doda�em do kodu kilka dyrektyw preprocesora


napisa�em plik wsadowy set_up_filter.bat, kt�ry nale�y uruchomi� jako administrator.
skopiuje on filtr do system32 i zatrejestruje

teraz w ustawieniach projektu w post operacjach wy��czy�em bezmy�ln� pr�b� zarejestrowania filtru i nie sypie b��dami


od teraz serwer uruchamia si� dopiero przy rozpocz�ciu transformacji,
�eby mo�na by�o przed uruchomieniem ustawi� we w�a�ciwo�ciach inny port tcp/ip


repozytorium: https://github.com/sigrond/Valve3

linkowa�a si� z�a wersja strmbase.lib
podmieni�em w D:\Program Files\Windows Kits\8.1\Lib\winv6.3\um\x86 na moj� wersj�