cls
echo arg1 - %1
echo arg2 - %2

"c:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"^
 -b^
 %1^
 --filename-format "{title}_{tag}_{tagframe}.{extension}"^
 --save-as %2

pause