# kritzel [WIP]  
Soon to be a simple drawing application.

---

### Features
- Canvas size equals window size
- No graphical user interface

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

---

## Dependencies
```
gtk3-devel goocanvas2-devel
```
