// -------------------------------------------------------------------------------------
// This is the main file for the circle test application
// -------------------------------------------------------------------------------------

#include "DXUT/DXUT.h"
#include "DXUT/DXUTmisc.h"
#include "resource.h"
#include "PrimitivePainter.h"
#include "NavigationGraph.h"

#include <math.h>


// The renderer we use.
PrimitivePainter m_paintingDevice;
// The navigation graph used.
NavigationGraph m_navGraph;


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable(  D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Create the nav graph.
	m_navGraph.GenerateNavGraph();


    // Set DXUT callbacks
	DXUTSetCallbackD3D9DeviceReset(OnD3D9ResetDevice);
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackMouse (OnMouse, true);

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"A Star" );
    DXUTCreateDevice( true, 720, 720 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}

// Handle the window size change.
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr = S_OK;
	pd3dDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ZENABLE, false);
	m_paintingDevice.MeasureScreen(pBackBufferSurfaceDesc);
 
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
    //
    // Clear the back buffer to black
    //
    pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_RGBA(0,0,0, 255), 0.0f, 0);
	pd3dDevice->BeginScene();

	m_paintingDevice.SetRenderingDevice(pd3dDevice);
	m_navGraph.PaintGraph(&m_paintingDevice);

	
	pd3dDevice->EndScene();
}




//--------------------------------------------------------------------------------------
// Update the scene here.
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	
}



//--------------------------------------------------------------------------------------
// Mouse presses.
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
	float position[2];
	if (bLeftButtonDown)
	{
		m_paintingDevice.ObtainMousePosition(xPos, yPos, position);
		m_navGraph.PickStartNode(position);
	}
	else if (bRightButtonDown)
	{
		m_navGraph.GenerateNavGraph();
	}
	else
	{
		m_paintingDevice.ObtainMousePosition(xPos, yPos, position);
		m_navGraph.PickDestination(position);
	}
}





