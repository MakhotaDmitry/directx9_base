#include "Resource.h"
#include "CBase.h"


class CGraphics2D
{
	private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		UINT					m_uintBackBufferWidth;
		UINT					m_uintBackBufferHeight;
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;
		int						m_iSizeVertexBufferInVertex;  // размер вершинного буфера в вершинах
		int						m_iSizeIndexBufferInNumber;  // размер индексного буфера в числах
		int						m_iCurIndexPrim;

		CBase					*m_pCBase;

		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProjection;
		D3DXMATRIX m_matRot;
		D3DXMATRIX m_matTrans;
		D3DXMATRIX m_matScale;
	public:
		CGraphics2D();
		~CGraphics2D();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight, CBase *base);
		void vRelease();
		void vBegin();
		void vEnd();


		void vDrawPoint(int x, int y, int z, D3DCOLOR color);
		void vDrawPointTest();
		void vDrawLine(int x1, int y1, int x2, int y2);
};