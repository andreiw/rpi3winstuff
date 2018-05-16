To build drivers you will need only the EWDK. Run the EWDK LaunchBuildEnv, navigate to a driver source directory and do:
```
buildme.cmd
```

WinPE
-----
Copy contents of `\drivers` and `\winpe_stuff` to `\Windows\system32` inside `\sources\boot.wim` of a WinPE image.

You will need to enable test signing in the bcd. E.g.:
   bcdedit /store K:\efi\Microsoft\boot\bcd /set {default} testsigning yes

