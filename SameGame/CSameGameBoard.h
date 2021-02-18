#pragma once
class CSameGameBoard
{
public:
	CSameGameBoard();
	~CSameGameBoard();
	// Функция для рандомной расстановки блоков в начале игры
	void SetupBoard();

	// Получаем цвет в определенном участке игрового поля
	COLORREF GetBoardSpace(int row, int col);

	// Геттеры для получения информации о параметрах игрового поля
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	int GetColumns() const { return m_nColumns; }
	int GetRows() const { return m_nRows; }
	
	// Мы закончили игру?
	bool IsGameOver(void) const;

	// Подсчет количества оставшихся блоков
	int GetRemainingCount(void) const { return m_nRemaining; }

	// Функция для удаления всех примыкающих блоков
	int DeleteBlocks(int row, int col);

	// Функция для удаления игрового поля и освобождения памяти
	void DeleteBoard();
	
	// Геттеры и сеттеры для количества цветов
	int GetNumColors() { return m_nColors; }
	void SetNumColors(int nColors) {
		m_nColors = (nColors >= 3 && nColors <= 7) ? nColors : m_nColors;
	}


private:
	// Функция для создания игрового поля и выделения памяти под него
	void CreateBoard();

	// Перечисление с вариантами направления (откуда мы пришли) потребуется для удаления блоков
	enum Direction
	{
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT
	};

	// Вспомогательная рекурсивная функция для удаления примыкающих блоков
	int DeleteNeighborBlocks(int row, int col, int color, Direction direction);

	// Функция для сжатия доски после того, как были удалены блоки
	void CompactBoard(void);

	// Указатель на двумерный массив
	int** m_arrBoard;

	// Список цветов, 0 – это цвет фона, 1-7 – это цвета блоков
	static COLORREF m_arrColors[8];

	// Информация о размере игрового поля
	int m_nColumns;
	int m_nRows;
	int m_nHeight;
	int m_nWidth;
	// Количество оставшихся блоков
	int m_nRemaining;
	// Количество цветов
	int m_nColors;
};

