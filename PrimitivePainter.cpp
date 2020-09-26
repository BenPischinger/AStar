#include "PrimitivePainter.h"
#include <math.h>

// The primitive we use for rendering.
DWORD PrimitivePainter::ScreenVertex::FVF =  D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

void PrimitivePainter::MeasureScreen(  const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	m_windowWidth = pBackBufferSurfaceDesc->Width;
	m_windowHeight = pBackBufferSurfaceDesc->Height;
}

// Draws a circle with the indicated radius and center onto the screen.
void  PrimitivePainter::PaintCircle(const float* centerPoint, const float* color)
{
	const int numOfSegments = 20;
	const float angleDelta = 2.0f * D3DX_PI / numOfSegments;
	// Yellow circle.
	const DWORD paintColor = D3DCOLOR_RGBA((int)(255 * color[0]),(int)(255 * color[1]),(int)(255 * color[2]), 255);
	float x,y;
	
	for(int i = 0; i <= numOfSegments; ++i)
	{
		x = centerPoint[0] + 0.01f * cosf(i * angleDelta);
		y = centerPoint[1] + 0.01f * sinf(i * angleDelta);
		m_vertexBuffer[i].color = paintColor;
		m_vertexBuffer[i].z = 0.5f;
		m_vertexBuffer[i].h = 1.0f;
		m_vertexBuffer[i].x = x * m_windowWidth;
		m_vertexBuffer[i].y = y * m_windowHeight;
	}
	m_baseDevice->SetFVF( ScreenVertex::FVF );
	m_baseDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, numOfSegments, m_vertexBuffer, sizeof(ScreenVertex));
}


// Paints a line from a given starting point to the indicated direction.
// startPoint: One point on the line.
// direction: directional vector that points along the line.
void PrimitivePainter::PaintLine(const float* startPoint, const float* endPoint,  bool solutionLink)
{
	DWORD paintColor;

	if (solutionLink)
		paintColor = D3DCOLOR_RGBA(128,64,255, 255);
	else
		paintColor = D3DCOLOR_RGBA(128,64,64, 255);
	m_vertexBuffer[0].color = paintColor;
	m_vertexBuffer[0].z = 0.5f;
	m_vertexBuffer[0].h = 1.0f;
	m_vertexBuffer[0].x = startPoint[0]  * m_windowWidth;
	m_vertexBuffer[0].y = startPoint[1]  * m_windowHeight;

	m_vertexBuffer[1] = m_vertexBuffer[0];
	m_vertexBuffer[1].x = endPoint[0]  * m_windowWidth;
	m_vertexBuffer[1].y = endPoint[1]  * m_windowHeight;

	m_baseDevice->SetFVF( ScreenVertex::FVF );
	m_baseDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, m_vertexBuffer, sizeof(ScreenVertex));
}

// Obtains corrected position for mouse.
void PrimitivePainter::ObtainMousePosition(int xPos, int yPos, float screenCoords[2])
{
	screenCoords[0] = (float)xPos / m_windowWidth;
	screenCoords[1] = (float)yPos / m_windowHeight;
}
