﻿//-----------------------------------------------------------------------------
// GGUI处理用户的输入
// (C) oil
// 2012-09-26
//-----------------------------------------------------------------------------
#include "GGUIInputProcess.h"
#include "GGUIWindowManager.h"
//-----------------------------------------------------------------------------
namespace GGUI
{
	GGUIInputProcess* GGUIInputProcess::ms_pInstance = 0;
	//-----------------------------------------------------------------------------
	GGUIInputProcess::GGUIInputProcess()
	:m_eCurrentMouseOpState(MouseOp_None)
	,m_nMousePosX(0)
	,m_nMousePosY(0)
	,m_theWindowContainMouse(Invalid_WindowID)
	{
		ms_pInstance = this;
	}
	//-----------------------------------------------------------------------------
	GGUIInputProcess::~GGUIInputProcess()
	{
		ReleaseInputProcess();
		ms_pInstance = 0;
	}
	//-----------------------------------------------------------------------------
	bool GGUIInputProcess::InitInputProcess()
	{
		return true;
	}
	//-----------------------------------------------------------------------------
	void GGUIInputProcess::ReleaseInputProcess()
	{

	}
	//-----------------------------------------------------------------------------
	bool GGUIInputProcess::OnMouseMove(SoInt nNewPosX, SoInt nNewPosY)
	{
		m_nMousePosX = nNewPosX;
		m_nMousePosY = nNewPosY;
		//
		bool bResult = false;
		//判断鼠标是否落在了用户窗口的外面。
		bool bOutOfScreen = IsMouseOutOfScreen(nNewPosX, nNewPosY);
		//判断鼠标落在了哪个窗口内部。
		WindowID theOldWindowWhoContainMouse = m_theWindowContainMouse;
		WindowID theNewWindowWhoContainMouse = Invalid_WindowID;
		if (!bOutOfScreen)
		{
			//如果之前鼠标落在了一个窗口内，则先判断鼠标是否仍然落在那个窗口内。
			//这里没有考虑那个窗口是否有子窗口。
			if (theOldWindowWhoContainMouse != Invalid_WindowID)
			{
				GGUIWindow* pOldWindow = GGUIWindowManager::GetInstance()->GetUIWindow(theOldWindowWhoContainMouse);
				if (pOldWindow && pOldWindow->GetVisible() && pOldWindow->CheckMouseInWindowArea(nNewPosX, nNewPosY))
				{
					theNewWindowWhoContainMouse = theOldWindowWhoContainMouse;
				}
			}
			//如果不确定鼠标落在哪个窗口内，则遍历所有的窗口。
			if (theNewWindowWhoContainMouse == Invalid_WindowID)
			{
				theNewWindowWhoContainMouse = GetWindowWhoContainMouse(nNewPosX, nNewPosY);
			}
		}
		//处理“鼠标落入了一个窗口矩形范围”的逻辑。
		if (theOldWindowWhoContainMouse != theNewWindowWhoContainMouse)
		{
			m_theWindowContainMouse = theNewWindowWhoContainMouse;
			if (theOldWindowWhoContainMouse != Invalid_WindowID)
			{
				MouseLeaveWindowArea(theOldWindowWhoContainMouse);
				bResult = true;
			}
			if (theNewWindowWhoContainMouse != Invalid_WindowID)
			{
				MouseEnterWindowArea(theNewWindowWhoContainMouse);
				bResult = true;
			}
		}
		//
		return bResult;
	}
	//-----------------------------------------------------------------------------
	bool GGUIInputProcess::OnMouseLeftButtonDown()
	{
		bool bResult = false;
		if (m_theWindowContainMouse != Invalid_WindowID)
		{
			GGUIWindow* pTheWindow = GGUIWindowManager::GetInstance()->GetUIWindow(m_theWindowContainMouse);
			if (pTheWindow && pTheWindow->GetEnable())
			{
				pTheWindow->OnMouseLeftButtonClickDown();
			}
		}
		return bResult;
	}
	//-----------------------------------------------------------------------------
	bool GGUIInputProcess::OnMouseLeftButtonUp()
	{
		bool bResult = false;
		if (m_theWindowContainMouse != Invalid_WindowID)
		{
			GGUIWindow* pTheWindow = GGUIWindowManager::GetInstance()->GetUIWindow(m_theWindowContainMouse);
			if (pTheWindow && pTheWindow->GetEnable())
			{
				pTheWindow->OnMouseLeftButtonClickUp();
			}
		}
		return bResult;
	}
	//-----------------------------------------------------------------------------
	void GGUIInputProcess::MouseEnterWindowArea(WindowID theWindow)
	{
		GGUIWindow* pWindow = GGUIWindowManager::GetInstance()->GetUIWindow(theWindow);
		if (pWindow)
		{
			pWindow->OnMouseEnterWindowArea();
		}
	}
	//-----------------------------------------------------------------------------
	void GGUIInputProcess::MouseLeaveWindowArea(WindowID theWindow)
	{
		GGUIWindow* pWindow = GGUIWindowManager::GetInstance()->GetUIWindow(theWindow);
		if (pWindow)
		{
			pWindow->OnMouseLeaveWindowArea();
		}
	}
	//-----------------------------------------------------------------------------
	WindowID GGUIInputProcess::GetWindowWhoContainMouse(SoInt nMousePosX, SoInt nMousePosY)
	{
		WindowID theResult = Invalid_WindowID;
		GGUIWindowManager* pWindowContainer = GGUIWindowManager::GetInstance();
		int nIndex = 0;
		GGUIWindow* pWindow = SoNULL;
		while (pWindowContainer->Next(nIndex, pWindow))
		{
			if (pWindow->GetVisible() && pWindow->CheckMouseInWindowArea(nMousePosX, nMousePosY))
			{
				theResult = pWindow->GetWindowID();
				break;
			}
		}
		return theResult;
	}
}
//-----------------------------------------------------------------------------
