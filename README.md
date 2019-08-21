# Prerequisites
[Cleo library 4.1.1.30](https://cleo.li/cleo4/CLEO4.1_setup.exe) (Won't work with *4.3*)  
[Samp 0.3.7 R1](http://files.sa-mp.com/sa-mp-0.3.7-install.exe) (Won't work with *R2/R3/DL*)  
Some experience with using Cleo in general.

# Installation
1. Install [Silent's ASI Loader](https://www.gtagarage.com/mods/show.php?id=21709)  
2. Put samp_commands.asi inside your Gta directory.  

# What it allows you to do 
Register commands without Sampfuncs plugin.  
Make custom command prefix (instead of the default forward slash).   
```
{$CLEO .cs}
0000: NOP

0AB1: @wait_till_game_and_samp_commands_loaded 0
0AB1: @register_commands_from_label 1 @cmd_store

while true 
wait 0 

end

:cmd_store
hex
">mycommand" 00 @my_1st_cmd     
"*mycommand" 00 @my_2nd_cmd

00 // this 00 must be just before end
end


:my_1st_cmd
if 0AB1: @get_last_command_params 0 _returned_string 3@
then
    0AD1: show_formatted_text_highpriority "my_1st_cmd params = ~G~%s" time 2000 3@
else 
    0AD1: show_formatted_text_highpriority "my_1st_cmd didn't have any params" time 2000
end
0AB1: @cmd_ret 0

:my_2nd_cmd
if 0AB1: @get_last_command_params 0 _returned_string 3@
then
    0AD1: show_formatted_text_highpriority "my_2nd_cmd params = ~G~%s" time 2000 3@
else 
    0AD1: show_formatted_text_highpriority "my_2nd_cmd didn't have any params" time 2000
end
0AB1: @cmd_ret 0


{This is a part of the code, see "Examples" folder for full example (where functions like "get_last_command_params", "register_commands_from_label" or "cmd_ret" are implemented)}
```

# Considerations  
In the current state it doesn't allow to register commands with the default forward slash.  
User is responsible for keeping track of command names to avoid registering the same command twice.  

# Credits 
Thanks to 0x688 for [Do your own SA:MP command's tutorial](http://ugbase.eu/index.php?threads/do-your-own-sa-mp-commands.18694/), it's very seful.

# Additional info
It was created with Visual Studio 2019 Community edition. If you're copying the code instead of opening/modifying the original project then go add `_CRT_SECURE_NO_WARNINGS` line to `Project -> Properties (at the bottom) -> C/C++ -> Preprocessor -> Preprocessor Definitions -> <Edit>`. Otherwise it won't compile well (because of strcpy). I also recommend to change `Output directory` to your Gta installation folder. This way you don't have to copy any files after each compilation. It's in:  
`Project -> Properties -> Configuration Properties -> General`

Also, in the `Configuration Poperties -> Advanced` you can set the `Target file extension` to `.asi` (which I did for this project). This way you don't have to bother with changing "dll" to "asi" after each compilation.