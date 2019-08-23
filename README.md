# Prerequisites
Cleo Library (works well with: [4.1.1.30](https://cleo.li/cleo4/CLEO4.1_setup.exe):heavy_check_mark: [4.3.22](https://cleo.li/cleo4/CLEO4_setup.exe):heavy_check_mark:)  
Samp (works well with: [0.3.7 R1](http://files.sa-mp.com/sa-mp-0.3.7-install.exe):heavy_check_mark: [0.3.7 R2](http://files.sa-mp.com/sa-mp-0.3.7-R2-install.exe):heavy_check_mark: [0.3.7 R3 1](http://files.sa-mp.com/sa-mp-0.3.7-R3-1-install.exe):heavy_check_mark: [0.3.DL R1](http://files.sa-mp.com/sa-mp-0.3.DL-R1-install.exe):heavy_check_mark:)  
Some experience with using Cleo in general.

# Installation
Put samp_commands.asi inside your Gta directory. Put the contents of the [functions.txt](https://github.com/michalmonday/samp_commands/blob/master/functions.txt) at the end of your Cleo script.  

# Usage 
Registering commands:
```
0AB1: @register_commands_from_label 1 @cmd_store
jump @your_code

:cmd_store
hex
"*my_first_command" 00 @my_1st_cmd     
"/another_command" 00 @my_2nd_cmd

00 // this 00 must be just before end
end
```

Command return:
```
0AB1: @cmd_ret 0
```

Getting params of last command
```
0AB1: @get_last_command_params 0 _returned_string 0@ 
```
> Note that there's no need to allocate memory to 0@, it points to 1000 bytes of allocated memory starting with input string (don't free it). It's equivalent to `0B35: samp 0@ = get_last_command_params`.  

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
">1" 00 @my_1st_cmd     
"*2" 00 @my_2nd_cmd

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


{Here you should put the content of functions.txt. See "Examples" folder for full example (where functions like "get_last_command_params", "register_commands_from_label" or "cmd_ret" are implemented)}
```

# Considerations  
At the moment the user is responsible for keeping track of command names to avoid registering the same command twice.  

# Credits/resources used
Thanks to [0x688](http://ugbase.eu/index.php?members/0x688.2/) for [Do your own SA:MP command's tutorial](http://ugbase.eu/index.php?threads/do-your-own-sa-mp-commands.18694/), it's very seful.  
Thanks to [Luchare](https://github.com/LUCHARE/) for [SAMP-API](https://github.com/BlastHackNet/SAMP-API) and developers of [mod_s0beit_sa](https://github.com/BlastHackNet/mod_s0beit_sa-1)

# Additional info
[Virus Total scan of samp_commands.asi](https://www.virustotal.com/gui/file/cf2f5e0020cf821a7deccf700dc4a24553395f5f49815f9488fdaf99b1531771/detection)  
It was created with Visual Studio 2019 Community edition. If you're copying the code instead of opening/modifying the original project then go add `_CRT_SECURE_NO_WARNINGS` line to `Project -> Properties (at the bottom) -> C/C++ -> Preprocessor -> Preprocessor Definitions -> <Edit>`. Otherwise it won't compile well (because of strcpy). I also recommend to change `Output directory` to your Gta installation folder. This way you don't have to copy any files after each compilation. It's in:  
`Project -> Properties -> Configuration Properties -> General`

Also, in the `Configuration Poperties -> Advanced` you can set the `Target file extension` to `.asi` (which I did for this project). This way you don't have to bother with changing "dll" to "asi" after each compilation.