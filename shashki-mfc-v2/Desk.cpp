// Desk.cpp: файл реализации
//

#include "pch.h"
#include "shashki-mfc-v2.h"
#include "afxdialogex.h"
#include "Desk.h"

void Desk::setPlayers(int role) {	
	if (role == 1) 
		human = Role::White;	
	else if (role == 2) 
		human = Role::Black;	

	board = BoardState::initialBoard();
	if (human == Role::Black)
		board = playNeiron(board, neiro);
}
// Диалоговое окно Desk

IMPLEMENT_DYNAMIC(Desk, CDialogEx)

Desk::Desk(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
}

Desk::~Desk()
{
	GdiplusShutdown(token);
}

void Desk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Desk, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// Обработчики сообщений Desk


void Desk::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);


	ItemWidth = double(rect.Width()),
		ItemHeight = double(rect.Height());
	//параметры графика в мировых кооринатах
	top = ItemHeight;			//2 * 4 * (1 + 0.2);
	bottom = 0;
	left = 0;
	right = ItemWidth;		//2 * 4 * (1 + 0.2);

	
	steplenY = (top - bottom) / 8,
	steplenX = (right - left) / 8;

	//xScale = ItemWidth / (right - left);
	//yScale = -ItemHeight / (top - bottom);

	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//сглаживание
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//преобразование мира(переход к координатам самого графика
	//Matrix matr;
	//matr.Scale(xScale, (yScale)); //маштабирование
	//matr.Translate(right / 2, -top / 2); //перенос начала координат

	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);
	Pen pen2(Color::Green, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::DarkGray);
	SolidBrush brushSh(Color::Aqua);
	SolidBrush brushBlack(Color::Black);
	SolidBrush brushWhite(Color::White);
	SolidBrush brushRed(Color::Red);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);
	
	//закрашиваем черные клетки
	double cleft = left, cright = left + steplenX;
	
	for (int i = 0; i < 8; i++) {
		double cbot = top - steplenY, ctop = top;
		for (int j = 0; j < 8; j++) {			
			if ((j + i) % 2) {
				gr.FillRectangle(&brush, RectF(cleft, cbot, steplenX, steplenY));
			}
			cbot -= steplenY;
			ctop -= steplenY;
		}
		cleft += steplenX;
		cright += steplenX;
	}

	//обработка клика мыши
	if (Click) {
		gr.FillRectangle(&brushSh, RectF(leftx, boty, steplenX, steplenY));
	}

	//BOARD
	//получили доску
	Board = GetBoardMas(board);
	//отрисовали
	cleft = left, cright = left + steplenX;

	for (int i = 0; i < 8; i++) {
		double cbot = bottom, ctop = top + steplenY;
		for (int j = 0; j < 8; j++) {
			if (Board[i][j] == 1) {				
				gr.FillEllipse(&brushWhite, RectF(cleft, cbot, steplenX, steplenY));				
			}
			else if (Board[i][j] == 2) {
				gr.FillEllipse(&brushWhite, RectF(cleft, cbot, steplenX, steplenY));
				gr.FillEllipse(&brushRed, RectF(cleft + steplenX / 4, cbot + steplenY / 4, steplenX / 2, steplenY / 2));
			}
			else if (Board[i][j] == -1) {
				gr.FillEllipse(&brushBlack, RectF(cleft, cbot, steplenX, steplenY));			
			}
			else if (Board[i][j] == -2) {
				gr.FillEllipse(&brushBlack, RectF(cleft, cbot, steplenX, steplenY));
				gr.FillEllipse(&brushRed, RectF(cleft + steplenX / 4, cbot + steplenY / 4, steplenX / 2, steplenY / 2));
			}
			
			cbot += steplenY;
			ctop += steplenY;
		}
		cleft += steplenX;
		cright += steplenX;
	}

	//Разметка
	for (int i = 0; i < 8; i++)
	{
		//горизонтальная
		gr.DrawLine(&BackGroundPen, PointF(left, top - i * steplenY), PointF(right , top  - i * steplenY));

		//вертикальная
		gr.DrawLine(&BackGroundPen, PointF(left + i * steplenX, top), PointF(left + i * steplenX, bottom));
	}

	//Отрисовка графика


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	Graphics grnew(dc);
	grnew.DrawImage(&Image, 0, 0);
}

//находит выбранную клетку
void Desk::FindCell(int& col, int& line) {
	leftx = left, rightx = left + steplenX;
	boty = bottom, topy = bottom + steplenY;
	
	col = 0; line = 0;	//ид выбранной колонки или линии

	for (int i = 1; i < 8; i++) {
		if ((xClick >= leftx) && (xClick <= rightx)) {
			col = i;
			i = 10;
		}
			
		else {
			leftx += steplenX;
			rightx += steplenX;
		}		
	}

	for (int i = 1; i < 8; i++) {
		if ((yClick >= boty) && (yClick <= topy)) {
			line = i;
			i = 10;
		}
			
		else {
			boty += steplenY;
			topy += steplenY;
		}		
	}

	if (col == 0)
		col = 7;
	else
		col--;
	if (line == 0)
		line = 7;
	else
		line--;
}

void Desk::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
	xClick = point.x;
	yClick = point.y;

	if(!Click)
		FindCell(col, line);			//находим шашку, которой будем ходить
	else
		FindCell(colStep, lineStep);	//находим место, куда будем ходить

	double color = 1;
	if (human == Role::Black)
		color = -1;

	//если уже выбрана шашка 
	if (Click && (Board[colStep][lineStep] == 0) && (!((colStep + lineStep) % 2))) {

		//если ходит шашка и не нужно есть
		if ((!king) && (abs(col - colStep) == 1) && (abs(line - lineStep) == 1) && board.quiet())
			HumanQuiet();

		else if ((!king) && (abs(col - colStep) == 2) && (abs(line - lineStep) == 2) && (!board.quiet()))
			HumanHungry();

		else if (king && board.quiet() && (abs(col - colStep) - abs(line - lineStep) == 0))
			HumanQuietKing();

		else if (king && !board.quiet() && (abs(col - colStep) - abs(line - lineStep) == 0))
			HumanHungryKing();		
		
		Click = false;
	}

	//выбираем только наши шашки
	else if ((!Click) && (Board[col][line] != 0) && (!((col + line) % 2)) && (Board[col][line] * color > 0)) {
		curCell = Cell::Cell(col, line);

		//проверка на дамку
		if (abs(Board[col][line]) == 2)
			king = true;

		Click = true;
	}
	else
		Click = false;
		
	
	Invalidate(false);

	CDialogEx::OnLButtonUp(nFlags, point);
}


//ход игрока, когда надо есть
void Desk::HumanHungry() {
	int dcol = col - colStep;
	int dline = line - lineStep;

	//if (color == Role::White) {
	//	if (word == "left") return Direction::LeftForward;
	//	if (word == "right") return Direction::RightForward;
	//	if (word == "left-back") return Direction::LeftBackward;
	//	if (word == "right-back") return Direction::RightBackward;
	//}
	//if (color == Role::Black) {
	//	if (word == "left") return Direction::LeftBackward;
	//	if (word == "right") return Direction::RightBackward;
	//	if (word == "left-back") return Direction::LeftForward;
	//	if (word == "right-back") return Direction::RightForward;
	//}

	//направление
	Direction d;

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Right");
		d = Direction::RightForward;
	}
	else if ((dcol > 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Left");
		d = Direction::LeftForward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::White) {
		//MessageBox(L"Left-Back");
		d = Direction::LeftBackward;
	}
	else if ((dcol < 0) && (dline > 0) && human == Role::White) {
		//MessageBox(L"Right-Back");
		d = Direction::RightBackward;
	}

	//выбор нарпавления для черных
	if ((dcol < 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Right");
		d = Direction::RightBackward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Left");
		d = Direction::LeftBackward;
	}
	else if ((dcol < 0) && (dline < 0) && human == Role::Black) {
		//MessageBox(L"Right-back");
		d = Direction::LeftForward;
	}
	else if ((dcol > 0) && (dline < 0) && human == Role::Black) {
		//MessageBox(L"Left-back");
		d = Direction::LeftForward;
	}
	//сам ход
	board.control(curCell);
	board.control(board.place().neighbour(d));
	
	while (!board.capture())	//посмотри, что делает
		board.control(board.place().neighbour(d));

	board.control(board.place().neighbour(d));
	
	board.control(Cell());	//эта штука хз что делает

	//ход за противника
	//board = playAutomatic(board);
	board = playNeiron(board, neiro);
}

//ход игрока, когда не надо есть
void Desk::HumanQuiet() {
	int dcol = col - colStep;
	int dline = line - lineStep;

	//if (color == Role::White) {
	//	if (word == "left") return Direction::LeftForward;
	//	if (word == "right") return Direction::RightForward;
	//	if (word == "left-back") return Direction::LeftBackward;
	//	if (word == "right-back") return Direction::RightBackward;
	//}
	//if (color == Role::Black) {
	//	if (word == "left") return Direction::LeftBackward;
	//	if (word == "right") return Direction::RightBackward;
	//	if (word == "left-back") return Direction::LeftForward;
	//	if (word == "right-back") return Direction::RightForward;
	//}

	//направление
	Direction d;

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Right");
		d = Direction::RightForward;

	}
	else if ((dcol > 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Left");
		d = Direction::LeftForward;
	}

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Right");
		d = Direction::RightBackward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Left");
		d = Direction::LeftBackward;
	}

	//сам ход
	board.control(curCell);
	board.control(board.place().neighbour(d));
	board.control(Cell());	//эта штука хз что делает

	//ход за противника
	//board = playAutomatic(board);
	board = playNeiron(board, neiro);
}

//ход игрока, когда не надо есть дамкой
void Desk::HumanQuietKing() {
	int dcol = col - colStep;
	int dline = line - lineStep;
	

	//направление
	Direction d;

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Right");
		d = Direction::RightForward;

	}
	else if ((dcol > 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Left");
		d = Direction::LeftForward;
	}

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Right");
		d = Direction::RightBackward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Left");
		d = Direction::LeftBackward;
	}

	//сам ход
	board.control(curCell);
	while (dline) {//в этом цикле ходит по одной клетке пока не дойдет
		board.control(board.place().neighbour(d));
		dline--;
	}
	
	board.control(Cell());	//эта штука хз что делает

	//ход за противника
	//board = playAutomatic(board);
	board = playNeiron(board, neiro);
}

//ход игрока, когда надо есть дамкой
void Desk::HumanHungryKing() {
	int dcol = col - colStep;
	int dline = line - lineStep;

	//if (color == Role::White) {
	//	if (word == "left") return Direction::LeftForward;
	//	if (word == "right") return Direction::RightForward;
	//	if (word == "left-back") return Direction::LeftBackward;
	//	if (word == "right-back") return Direction::RightBackward;
	//}
	//if (color == Role::Black) {
	//	if (word == "left") return Direction::LeftBackward;
	//	if (word == "right") return Direction::RightBackward;
	//	if (word == "left-back") return Direction::LeftForward;
	//	if (word == "right-back") return Direction::RightForward;
	//}

	//направление
	Direction d;

	//выбор нарпавления для белых 
	if ((dcol < 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Right");
		d = Direction::RightForward;
	}
	else if ((dcol > 0) && (dline < 0) && human == Role::White) {
		//MessageBox(L"Left");
		d = Direction::LeftForward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::White) {
		//MessageBox(L"Left-Back");
		d = Direction::LeftBackward;
	}
	else if ((dcol < 0) && (dline > 0) && human == Role::White) {
		//MessageBox(L"Right-Back");
		d = Direction::RightBackward;
	}

	//выбор нарпавления для черных
	if ((dcol < 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Right");
		d = Direction::RightBackward;
	}
	else if ((dcol > 0) && (dline > 0) && human == Role::Black) {
		//MessageBox(L"Left");
		d = Direction::LeftBackward;
	}
	else if ((dcol < 0) && (dline < 0) && human == Role::Black) {
		//MessageBox(L"Right-back");
		d = Direction::LeftForward;
	}
	else if ((dcol > 0) && (dline < 0) && human == Role::Black) {
		//MessageBox(L"Left-back");
		d = Direction::LeftForward;
	}
	//сам ход
	board.control(curCell);
	board.control(board.place().neighbour(d));

	while (!board.capture())	//посмотри, что делает
		board.control(board.place().neighbour(d));
	dline--;

	while (dline) {//в этом цикле ходит по одной клетке пока не дойдет
		board.control(board.place().neighbour(d));
		dline--;
	}

	board.control(Cell());	//эта штука хз что делает

	//ход за противника
	//board = playAutomatic(board);
	board = playNeiron(board, neiro);
}