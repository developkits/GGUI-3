﻿//-----------------------------------------------------------------------------
// (C) oil
// 2012-10-21
//-----------------------------------------------------------------------------
#ifndef _GGUIDXRenderManager_h_
#define _GGUIDXRenderManager_h_
//-----------------------------------------------------------------------------
#include "GGUID3DDefine.h"
#include "GGUIBaseDefine.h"
//-----------------------------------------------------------------------------
namespace GGUI
{
	//-----------------------------------------------------------------------------
	class GGUIDXRenderManager
	{
	public:
		GGUIDXRenderManager();
		~GGUIDXRenderManager();
		static GGUIDXRenderManager* GetInstance();
		//
		bool InitDXRenderManager();
		//
		void PreRender();
		void AddRnederUnit(const stRenderUnit& theRenderUnit);
		void DoRender();
		void PostRender();

	private:
		//创建DX顶点缓冲区。
		bool CreateDXVertexBuffer(int nCapacity);
		//创建DX索引缓冲区。
		bool CreateDXIndexBuffer(int nCapacity);

	private:
		//顶点数据格式。
		struct stVertexContent
		{
			float fX, fY, fZ, fRHW;
			unsigned int uiColor;
			float fCoordX, fCoordY;
		};

	private:
		static GGUIDXRenderManager* ms_pInstance;
	private:
		//DX顶点缓冲区。
		//顶点缓冲区会频繁做写操作，所以适合D3DPOOL_DEFAULT和D3DUSAGE_WRITEONLY搭配使用。
		//需要主动处理设备丢失。当设备丢失时释放资源，设备重置后重新创建。
		IDirect3DVertexBuffer9* m_pDXVertexBuffer;
		//DX索引缓冲区。
		//索引缓冲区数据不会变化，所以使用D3DPOOL_MANAGED。
		//不需要处理设备丢失。
		IDirect3DIndexBuffer9* m_pDXIndexBuffer;
		//绘制前保存原来的DX设备渲染状态。绘制过后要还原为原来状态。
		IDirect3DStateBlock9* m_pDXStateBlock;
		//存储在一次绘制中要绘制哪些顶点。
		stVertexContent* m_arrayVertexContent;
		//记录DX顶点缓冲区的最大容量，最多容纳多少个顶点。
		int m_nDXVertexCapacity;
		//记录DX索引缓冲区的最大容量，最多容纳多少个三角形。
		int m_nDXIndexCapacity;
		//记录顶点数组中最多存储多少个元素。
		int m_nVertexContentCapacity;
		//记录数组中索引号最大的有效元素的下一个索引号。
		//如果数组中最后一个有效元素的下标为M，则该值为(M+1）。
		int m_nVertexContentIndexEnd;
		//记录本次绘制的纹理贴图ID。
		ImagesetID m_theTargetImagesetID;

	};
	//-----------------------------------------------------------------------------
	inline GGUIDXRenderManager* GGUIDXRenderManager::GetInstance()
	{
		return ms_pInstance;
	}
} //namespace GGUI
//-----------------------------------------------------------------------------
#endif //_GGUIDXRenderManager_h_
//-----------------------------------------------------------------------------
