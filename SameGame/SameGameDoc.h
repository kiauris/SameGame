
// SameGameDoc.h: интерфейс класса CSameGameDoc 
//


#pragma once

#include "CSameGameBoard.h"

class CSameGameDoc : public CDocument
{
protected: // создать только из сериализации
	CSameGameDoc() noexcept;
	DECLARE_DYNCREATE(CSameGameDoc)

// Атрибуты
public:

// Операции
public:

	// Геттеры для получения информации о параметрах игрового поля
	COLORREF GetBoardSpace(int row, int col) {
		return m_board.GetBoardSpace(row, col);
	}
	void SetupBoard() { m_board.SetupBoard(); }
	int GetWidth() const { return m_board.GetWidth(); }
	int GetHeight() const { return m_board.GetHeight(); }
	int GetColumns() const { return m_board.GetColumns(); }
	int GetRows() const { return m_board.GetRows(); }
	bool IsGameOver() { return m_board.IsGameOver(); }
	int DeleteBlocks(int row, int col) {
		return m_board.DeleteBlocks(row, col);
	}
	int GetRemainingCount() { return m_board.GetRemainingCount(); }
	void DeleteBoard() { m_board.DeleteBoard(); }
	void SetNumColors(int nColors);
	int GetNumColors() { return m_board.GetNumColors(); }

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CSameGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Экземпляр объекта нашей игровой доски
	CSameGameBoard m_board;

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
