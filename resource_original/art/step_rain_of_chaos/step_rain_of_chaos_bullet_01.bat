cls

set aseprite_file_name=%~n0.aseprite
set png_file_name=%~n0.png

call ../asepriteExporter_PNG_LayerMerge_Title_Frame.bat^
 %aseprite_file_name%^
 %png_file_name%