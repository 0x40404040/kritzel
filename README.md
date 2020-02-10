# kritzel   
Bare minimum drawing application.


Created in the need for a simple drawing application for scribbles.


kritzel is tested on Linux only. 
This may work on Windows and Mac.

---

### Features
- Canvas size equals window size
- No graphical user interface
- Undo last drawing
- Clear complete Canvas
- Eraser
- Max 4 Different Colors
- Increase/Decrease line with

### Not implemented 
- Save/Load images
- No canvas scrolling

---

## Keyboard shortcuts
| Keys      |                   							|
|:--------- | ---------------------------------------------	|
| Shift + r | Undo last drawing 							|
| Shift + c | Clear Canvas      							|
| 1 		| Select color 1      							|
| 2 		| Select color 2      							|
| 3 		| Select color 3      							|
| 4 		| Select color 4      							|
| e 		| (erase) select background color      			|
| a  		| (add) increase line width     				|
| s  		| (subtract) decrease line width      			|
| d 		| (default) set line width to default value     |


---

## Configuration
The following values can be changed in ``config.h``.
- Background color
- Color palette
- Line width
- Line vary when increment/decrement line width

Need to Recompile to update config.

---

## Dependencies
```
gtk3-devel goocanvas2-devel
```

