//tools
#define UM_PROPERTIES	0x01
#define UM_PEN			0x02
#define UM_PATH			0x04
#define UM_TRANSFORM	0x08
#define UM_CLIPAREA		0x10
#define UM_TEST			0x20

//generic tools mod
#define UMF_FREESHAPE	0x0000
#define UMF_RECTANGLE	0x1000
#define UMF_ELLIPSE		0x2000
#define UMF_VECTOR		0x4000
#define UMF_FILL		0x8000

//control tools mod : UM_PROPERTIES
#define UMF_ALL			0xFF00
#define UMF_FLAGS		0x0100
#define UMF_WIDTH		0x0200
#define UMF_COLOR		0x0400
#define UMF_HATCH		0x0800
#define UMF_STYLE		0x1000
#define UMF_DATA		0x2000
#define UMF_PENDATA		UMF_FLAGS | UMF_WIDTH | UMF_PACKED
#define UMF_BRUSH		UMF_COLOR | UMF_HATCH | UMF_STYLE | UMF_PACKED

//control tools mod : UM_TRANSFORM
#define UMF_MOVE		0x5
#define UMF_STRETCH		0x4
#define UMF_ROTATE		0x3

//control tools options
#define UMF_UPDATEVALUE	0x10000
#define UMF_PACKED		0x20000

//tools savestate
#define UMF_DONOTSAVE	0xFFFF0000
#define UMF_SAVESTATE	0x10000

#define UWM_SAVE			(WM_USER+1)

//file
#define UM_NEWFILE		0x1
#define UM_PASTEIMAGE	0x2
#define UMF_BUFFER		0x10000

//dialog box custom message
#define UDBM_VECTOR		(WM_USER+0x100)

//dialog box custom parameters
#define UDBF_RESERVE	0x1
#define UDBF_PUSHBACK	0x2