rem I use WSL, and the case=dir behavior breaks msbuild
fsutil.exe file setCaseSensitiveInfo . disable
msbuild /property:Platform=ARM64
