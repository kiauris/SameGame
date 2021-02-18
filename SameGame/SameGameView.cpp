
// SameGameView.cpp: реализация класса CSameGameView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"
#include "SameGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSameGameView

IMPLEMENT_DYNCREATE(CSameGameView, CView)

BEGIN_MESSAGE_MAP(CSameGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()

	ON_COMMAND(ID_LEVEL_3COLORS, &CSameGameView::OnLevel3colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_3COLORS, &CSameGameView::OnUpdateLevel3colors)
	ON_COMMAND(ID_LEVEL_4COLORS, &CSameGameView::OnLevel4colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_4COLORS, &CSameGameView::OnUpdateLevel4colors)
	ON_COMMAND(ID_LEVEL_5COLORS, &CSameGameView::OnLevel5colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_5COLORS, &CSameGameView::OnUpdateLevel5colors)
	ON_COMMAND(ID_LEVEL_6COLORS, &CSameGameView::OnLevel6colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_6COLORS, &CSameGameView::OnUpdateLevel6colors)
	ON_COMMAND(ID_LEVEL_7COLORS, &CSameGameView::OnLevel7colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_7COLORS, &CSameGameView::OnUpdateLevel7colors)
END_MESSAGE_MAP()

// Создание или уничтожение CSameGameView

CSameGameView::CSameGameView() noexcept
{
}

CSameGameView::~CSameGameView()
{
}

BOOL CSameGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Рисование CSameGameView

void CSameGameView::OnDraw(CDC* pDC)
{
	// В начале создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// Сохраняем текущее состояние контекста устройства
	int nDCSave = pDC->SaveDC();

	// Получаем размеры клиентской области
	// Получаем клиентский прямоугольник
	CRect rcClient;
	GetClientRect(&rcClient);
	// Получаем фоновый цвет доски
	COLORREF clr = pDoc->GetBoardSpace(-1, -1);

	// Сначала рисуем фон
	pDC->FillSolidRect(&rcClient, clr);

	// Создаём кисть для рисования
	CBrush br;
	br.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pbrOld = pDC->SelectObject(&br);

	// Рисуем квадраты
	for (int row = 0; row < pDoc->GetRows(); row++) {
		for (int col = 0; col < pDoc->GetColumns(); col++) 
		{
			// Получаем цвет для пространства доски
			clr = pDoc->GetBoardSpace(row, col);

			// Рассчитываем размер и положение пространства доски
			CRect rcBlock;
			rcBlock.top = row * pDoc->GetHeight();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();
			rcBlock.left = col * pDoc->GetWidth();
			rcBlock.right = rcBlock.left + pDoc->GetWidth();

			// Заполняем блок правильным цветом
			pDC->FillSolidRect(&rcBlock, clr);

			// Рисуем контур блока
			pDC->Rectangle(&rcBlock);
		}
	}

	// Восстановление настроек контекста устройства
	pDC->RestoreDC(nDCSave);
	br.DeleteObject();
}


// Диагностика CSameGameView

#ifdef _DEBUG
void CSameGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSameGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSameGameDoc* CSameGameView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSameGameDoc)));
	return (CSameGameDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CSameGameView


void CSameGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// Изменяем размеры окна
	ResizeWindow();
}

void CSameGameView::ResizeWindow()
{
	// Создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Получаем размер клиентской области и окна
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);

	// Вычисляем разницу
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();

	// Изменяем размер окна в соответствие с размером нашей доски
	rcWindow.right = rcWindow.left + pDoc->GetWidth()*pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top + pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff;

	// Функция MoveWindow() изменяет размер окна фрейма
	GetParentFrame()->MoveWindow(&rcWindow);
}


void CSameGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Вначале создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Получаем индекс строки и столбца элемента, по которому был осуществлен клик мышкой
	int row = point.y / pDoc->GetHeight();
	int col = point.x / pDoc->GetWidth();

	// Удаляем блоки из Document
	int count = pDoc->DeleteBlocks(row, col);

	// Проверяем, было ли удаление блоков
	if (count > 0) {
		// Перерисовываем View
		Invalidate();
		UpdateWindow();

		// Проверяем, закончилась ли игра
		if (pDoc->IsGameOver()) 
		{
			// Получаем количество оставшихся блоков
			int remaining = pDoc->GetRemainingCount();
			CString message;
			message.Format(_T("No more moves left\nBlocks remaining: %d"), remaining);

			// Отображаем пользователю результат игры
			MessageBox(message, _T("Game Over"), MB_OK | MB_ICONINFORMATION);

		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CSameGameView::setColorCount(int numColors)
{
	// Получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Устанавливаем количество цветов
	pDoc->SetNumColors(numColors);

	// Перерисовываем View
	Invalidate();
	UpdateWindow();
}

void CSameGameView::OnLevel3colors()
{
	setColorCount(3);
}


void CSameGameView::OnUpdateLevel3colors(CCmdUI *pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 3);
}


void CSameGameView::OnLevel4colors()
{
	setColorCount(4);
}


void CSameGameView::OnUpdateLevel4colors(CCmdUI *pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 4);
}


void CSameGameView::OnLevel5colors()
{
	setColorCount(5);
}


void CSameGameView::OnUpdateLevel5colors(CCmdUI *pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 5);
}


void CSameGameView::OnLevel6colors()
{
	setColorCount(6);
}


void CSameGameView::OnUpdateLevel6colors(CCmdUI *pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 6);
}


void CSameGameView::OnLevel7colors()
{
	setColorCount(7);
}


void CSameGameView::OnUpdateLevel7colors(CCmdUI *pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 7);
}
