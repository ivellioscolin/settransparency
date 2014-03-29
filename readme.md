Set Transparency
----------------------------
Set any window to transparency. Work for windows later than XP.

### Usage:<br/>
SetTransparency.exe &lt;pid&gt; &lt;alpha&gt; &lt;title&gt;  
>pid: Process ID  
>alpha: Alpha depth, 0-255  
>title: Sub-string of main window title. The sub-string is to identify the main window handle since hwnd is not unique.  

### Example:
SetTransparency.exe 1234 200 cmd  