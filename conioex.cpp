/**
* @file	conioex.h
* @brief	Win32 コンソール I/O 拡張 for Visual C++/Borland C++
*
* @date	2023/11 2023年度版
* @version	1.02
* @note
*  コンソールウィンドウのプロパティの設定を以下に変更すること
*  ・「従来のコンソールを使う」のチェックを外す
*  ・「簡易編集モード」のチェックを外す
*
* コンソール関数について
* https://learn.microsoft.com/ja-jp/windows/console/console-functions
* 
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "conioex.h"

#define CONSOLE_INPUT_MODE	(ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT)
#define CONSOLE_OUTPUT_MODE	(ENABLE_PROCESSED_OUTPUT|ENABLE_LVB_GRID_WORLDWIDE)

	typedef struct {
		int				device_type;
		MCIDEVICEID		device_id;
		char			path[MAX_PATH];
		int				repeat;
	} MciSoundInfo;

	HANDLE	display_handle[2];
	HANDLE	input_handle;
	DWORD	conio_key_map[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	COORD	conio_mouse_position = { 0, 0 };
	COORD	screen_buffer_size;
	//WORD	screen_buffer_attribute = LIGHTGRAY;
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	CONSOLE_FONT_INFOEX font_size = { sizeof font_size };
	SMALL_RECT window_size;
	int swap_flg = 0;
	int cursor_origin = 1;	// ウィンドウ左上端のカーソル座標。(1, 1)または(0, 0)

	CHAR_INFO* screen_buffer = NULL;

	/**
	 * @brief	コンソール I/O 初期化
	 *
	 * @param	width [入力] コンソールウィンドウの横サイズ(60～)
	 * @param	height [入力] コンソールウィンドウの縦サイズ(1～)
	 */
	COORD InitConio(int width, int height)
	{
		// ウィンドウサイズが最低値未満なら最低値に変更
		if (height < CONSOLE_WINDOW_HEIGHT_MIN) {
			height = CONSOLE_WINDOW_HEIGHT_MIN;
		}
		if (width < CONSOLE_WINDOW_WIDTH_MIN) {
			width = CONSOLE_WINDOW_WIDTH_MIN;
		}

		CONSOLE_HISTORY_INFO history_info{};

		// ディスプレイ情報取得
		display_handle[0] = GetStdHandle(STD_OUTPUT_HANDLE);
		display_handle[1] = NULL;
		input_handle = GetStdHandle(STD_INPUT_HANDLE);

		// コマンド履歴を保存しない
		history_info.cbSize = sizeof(CONSOLE_HISTORY_INFO);
		history_info.HistoryBufferSize = 0;
		history_info.NumberOfHistoryBuffers = 0;
		history_info.dwFlags = 0;
		if (SetConsoleHistoryInfo(&history_info) == 0) {
			printf("Failed SetConsoleHistoryInfo ErrorCode:%d\n", GetLastError());
		}

		// 一時的にフォントサイズを最小値に設定
		font_size.dwFontSize.X = 1;
		font_size.dwFontSize.Y = 1;
		if (SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size) == 0) {
			printf("Failed SetCurrentConsoleFontEx ErrorCode:%d\n", GetLastError());
		}

		// 現在のフォントサイズとディスプレイの解像度から、最大ウィンドウサイズを計算
		COORD maxSize = GetLargestConsoleWindowSize(display_handle[0]);
		if (width > maxSize.X) {
			width = maxSize.X;
		}
		if (height > maxSize.Y) {
			height = maxSize.Y;
		}

		// ウィンドウサイズ変更
		window_size.Left = 0;
		window_size.Top = 0;
		window_size.Right = width - 1;
		window_size.Bottom = height - 1;
		if (SetConsoleWindowInfo(display_handle[0], TRUE, &window_size) == 0) {
			printf("Failed SetConsoleWindowInfo ErrorCode:%d\n", GetLastError());
		}

		// バッファサイズ変更
		screen_buffer_size.X = width;
		screen_buffer_size.Y = height;
		if (SetConsoleScreenBufferSize(display_handle[0], screen_buffer_size) == 0) {
			printf("Failed SetConsoleScreenBufferSize ErrorCode:%d\n", GetLastError());
		}

		// ウィンドウサイズ再変更
		window_size.Left = 0;
		window_size.Top = 0;
		window_size.Right = width - 1;
		window_size.Bottom = height - 1;
		if (SetConsoleWindowInfo(display_handle[0], TRUE, &window_size) == 0) {
			printf("Failed SetConsoleWindowInfo ErrorCode:%d\n", GetLastError());
		}

		// コンソールウィンドウのモード設定
		if (SetConsoleMode(display_handle[0], CONSOLE_OUTPUT_MODE) == 0) {
			printf("Failed SetConsoleMode ErrorCode:%d\n", GetLastError());
		}
		if (SetConsoleMode(input_handle, CONSOLE_INPUT_MODE) == 0) {
			printf("Failed SetConsoleMode ErrorCode:%d\n", GetLastError());
		}

		// スクリーンバッファの情報を取得
		if (GetConsoleScreenBufferInfo(display_handle[0], &screen_buffer_info) == 0) {
			printf("Failed GetConsoleScreenBufferInfo ErrorCode:%d\n", GetLastError());
		}

		// スクリーンバッファ用の配列を作成
		screen_buffer = new CHAR_INFO[width * height];
		memset(screen_buffer, 0, (size_t)width * height);

		// フォントサイズを再設定
		font_size.dwFontSize.X = DEF_FONTSIZE_X;
		font_size.dwFontSize.Y = DEF_FONTSIZE_Y;
		if (SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size) == 0) {
			printf("Failed SetCurrentConsoleFontEx ErrorCode:%d\n", GetLastError());
		}
		return screen_buffer_size;
	}

	void UninitConio() {
		if (screen_buffer != NULL) {
			delete screen_buffer;
			screen_buffer = NULL;
		}
	}

	/**
	 * @brief	ダブルバッファ初期化
	 *
	 * @return	バッファハンドル取得失敗
	 */
	int InitDoubleBuffer(void)
	{
		// ダブルバッファ用のメモリーを確保
		display_handle[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		if (display_handle[0] == INVALID_HANDLE_VALUE) {
			printf("Failed CreateConsoleScreenBuffer display_handle[0]\n");
			return -1;
		}

		display_handle[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		if (display_handle[1] == INVALID_HANDLE_VALUE) {
			printf("Failed CreateConsoleScreenBuffer display_handle[1]\n");
			return -1;
		}

		// フォントサイズの変更
		SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size);
		SetCurrentConsoleFontEx(display_handle[1], TRUE, &font_size);

		SetConsoleScreenBufferSize(display_handle[0], screen_buffer_size);	// バッファサイズ変更
		SetConsoleWindowInfo(display_handle[0], TRUE, &window_size);		// ウィンドウサイズ変更

		SetConsoleScreenBufferSize(display_handle[1], screen_buffer_size);	// バッファサイズ変更
		SetConsoleWindowInfo(display_handle[1], TRUE, &window_size);		// ウィンドウサイズ変更

		SetConsoleMode(display_handle[0], CONSOLE_OUTPUT_MODE);	// バッファを上書きモードに
		SetConsoleMode(display_handle[1], CONSOLE_OUTPUT_MODE);	// バッファを上書きモードに

		return 0;
	}

	/**
	 * @brief	フォントサイズ変更
	 *
	 * @param	width [入力] フォントの横サイズ(1～)
	 * @param	height [入力] フォントの縦サイズ(1～)
	 */
	void SetScreenFontSize(int width, int height)
	{
		// フォントサイズ変更
		font_size.dwFontSize.X = width;
		font_size.dwFontSize.Y = height;
		if (display_handle[1] == NULL) {
			SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size);
		}
		else {
			SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size);
			SetCurrentConsoleFontEx(display_handle[1], TRUE, &font_size);
		}
	}

	/**
	 * @brief	水平方向のカーソル位置を取得
	 *
	 * @return	現在のカーソル位置のX座標(1～)または(0～)
	 */
	int GetCursorX(void)
	{
		return screen_buffer_info.dwCursorPosition.X - screen_buffer_info.srWindow.Left - cursor_origin;
	}

	/**
	 * @brief	垂直方向のカーソル位置を取得
	 *
	 * @return	現在のカーソル位置のY座標(1～)または(0～)
	 */
	int GetCursorY(void)
	{
		return screen_buffer_info.dwCursorPosition.Y - screen_buffer_info.srWindow.Top - cursor_origin;
	}

	/**
	 * @brief	カーソル位置の移動
	 *
	 * @param	x [入力] X座標(1～)または(0～)
	 * @param	y [入力] Y座標(1～)または(0～)
	 */
	void SetCursorPosition(int x, int y)
	{
		COORD lc{};

		lc.X = x - cursor_origin;
		lc.Y = screen_buffer_info.srWindow.Top + y - cursor_origin;

		SetConsoleCursorPosition(display_handle[swap_flg], lc);
	}

	/**
	 * @brief	水平方向のスクリーンサイズを取得
	 *
	 * @return	水平方向のスクリーンサイズ
	 */
	int GetScreenSizeX(void) {
		return screen_buffer_size.X;
	}

	/**
	 * @brief	垂直方向のスクリーンサイズを取得
	 *
	 * @return	垂直方向のスクリーンサイズ
	 */
	int GetScreenSizeY(void) {
		return screen_buffer_size.Y;
	}

	/**
	 * @brief	カーソル位置の原点を設定
	 *
	 * @param	flag [入力] trueなら原点を0にする、falseなら原点を1にする
	 */
	void SetCursorOriginZeroFlag(bool flag) {
		if (flag) {
			cursor_origin = 0;
		}
		else {
			cursor_origin = 1;
		}
	}

#ifdef UNICODE
	/**
	 * @brief	文字列の出力(マルチバイト⇒Unicode変換版)
	 *
	 * @param	_src [入力] 出力文字列配列のポインタ
	 * @param	_size [入力] 出力文字数
	 */
	void PrintString(const char* _src, int _size)
	{
		//指定されたマルチバイト文字全てをワイド文字(Unicode文字)変換した場合の
		//必要なバッファーサイズ (終端の null 文字を含む) を文字単位で算出し、
		//その文字数分のバッファーを確保する。
		int wc_src_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);
		size_t wc_src_bytes = (wc_src_siz * sizeof(wchar_t));	//Unicode文字換算のバイト数。
		wchar_t* wc_src = (wchar_t*)_malloca(wc_src_bytes);	//スタック上に確保：free不要
		if (wc_src == NULL) {
			return;
		}
		memset(wc_src, 0, wc_src_bytes);	//０クリア：書き込みが途中まででも'\0'終端文字列になる。
		//指定サイズが元の文字数をオーバーしている場合の補正
		if (strlen(_src) < _size) {
			_size = (-1);	//(-1)指定で'\0'まで変換。
		}
		//指定サイズ分変換する（_size == (-1))なら'\0'まで全て変換する）
		//戻り値は変換した(バッファに書き込まれた)文字数が返る。
		//【注】(-1)指定で変換した場合、戻り値は'\0'を含む文字数になる。
		int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, _size, wc_src, wc_src_siz);
		//disp_sizは'\0'を含む文字数かもしれないので、変換した文字数になる様に再計算する。
		disp_siz = (int)wcslen(wc_src);
		DWORD num;	//実際に書き込まれた文字数を受け取る変数
		WriteConsoleW(display_handle[swap_flg], wc_src, disp_siz, &num, NULL);
		//WriteConsoleW(display_handle[swap_flg], pWChar, ret_val, &num, NULL);
		//WCHAR wide_char[256];
		//memset(wide_char, 0, sizeof(wide_char));
		//ret_val = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, srcbuf, size, wide_char, size);
		//WriteConsole(display_handle[swap_flg], wide_char, ret_val, &num, NULL);
	}
#endif	//UNICODE
	/**
	 * @brief	文字列の出力（マルチバイト文字用）
	 *
	 * @param	_srcbuf [入力] 出力文字列配列のポインタ
	 * @param	_size [入力] 出力文字数
	 */
	void PrintStringA(const char* _srcbuf, int _size) {
		//表示文字数が指定文字列のサイズをオーバーしていたら補正する
		int src_len = (int)strlen(_srcbuf);
		if (_size < 0) {
			_size = src_len;
		}
		else if (_size > src_len) {
			_size = (int)strlen(_srcbuf);
		}
		DWORD num;
		WriteConsoleA(display_handle[swap_flg], _srcbuf, _size, &num, NULL);
	}
	/**
	 * @brief	文字列の出力（Unicode文字用）
	 *
	 * @param	_srcbuf [入力] 出力文字列配列のポインタ
	 * @param	_size [入力] 出力文字数
	 */
	void PrintStringW(const wchar_t* _srcbuf, int _size) {
		//表示文字数が指定文字列のサイズをオーバーしていたら補正する
		int src_len = (int)wcslen(_srcbuf);
		if (_size < 0) {
			_size = src_len;
		}
		else if (_size > src_len) {
			_size = (int)wcslen(_srcbuf);
		}
		DWORD num;
		WriteConsoleW(display_handle[swap_flg], _srcbuf, _size, &num, NULL);
	}
	//
	/**
	 * @brief	行末まで消去
	 */
	void ClearLine(void)
	{
		DWORD fill_num;

		FillConsoleOutputAttribute(display_handle[swap_flg],
			screen_buffer_info.wAttributes,
			screen_buffer_info.srWindow.Right - screen_buffer_info.dwCursorPosition.X + 1,
			screen_buffer_info.dwCursorPosition,
			&fill_num);
		FillConsoleOutputCharacter(display_handle[swap_flg],
			TEXT(' '),
			screen_buffer_info.srWindow.Right - screen_buffer_info.dwCursorPosition.X + 1,
			screen_buffer_info.dwCursorPosition,
			&fill_num);
	}

	/**
	 * @brief	画面消去
	 */
	void ClearScreen(void)
	{
		DWORD fill_num;
		COORD screen_origin = { 0, 0 };

		//screen_buffer_attribute = screen_buffer_info.wAttributes;

		FillConsoleOutputAttribute(display_handle[swap_flg],
			screen_buffer_info.wAttributes,
			screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y,
			screen_origin,
			&fill_num);
		FillConsoleOutputCharacter(display_handle[swap_flg],
			TEXT(' '),
			screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y,
			screen_origin,
			&fill_num);
		SetCursorPosition(screen_buffer_info.srWindow.Left + 1, screen_buffer_info.srWindow.Top + 1);
	}

	/**
	 * @brief	ダブルバッファ時の描画面切替
	 */
	void FlipScreen(void)
	{
		SetConsoleActiveScreenBuffer(display_handle[swap_flg]);	// バッファを入れ替え表示
		swap_flg = (swap_flg) ? 0 : 1;
	}

	/**
	 * @brief	文字色高輝度化
	 */
	void SetHighVideoColor(void)
	{
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes |= FOREGROUND_INTENSITY);
	}

	/**
	 * @brief	文字色低輝度化
	 */
	void SetLowVideoColor(void)
	{
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes &= ~FOREGROUND_INTENSITY);
	}

	/**
	 * @brief	既定文字色設定
	 */
	void SetNormalVideoColor(void)
	{
		//SetConsoleTextAttribute(display_handle[swap_flg], LIGHTGRAY);
	}

	/**
	 * @brief	文字背景色設定
	 *
	 * @param	color [入力] 文字背景色
	 * @note
	 *  背景色はenum COLORSを参照する
	 */
	void SetTextBackColor(int color)
	{
		screen_buffer_info.wAttributes &= ~0x00f0;
		//screen_buffer_info.wAttributes |= ((color & 0x07) << 4);
		screen_buffer_info.wAttributes |= ((color & 0x0F) << 4);
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes);
	}

	/**
	 * @brief	文字色設定
	 *
	 * @param	color [入力] 文字色
	 * @note
	 *  文字色はenum COLORSを参照する
	 */
	void SetConsoleTextColor(int color)
	{
		screen_buffer_info.wAttributes &= ~0x000f;
		screen_buffer_info.wAttributes |= (color & 0x0f);
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes);
	}

	/**
	 * @brief	文字色背景色同時設定
	 *
	 * @param	attribute [入力] 文字背景情報
	 * @note
	 *  以下の各設定値をビット毎のOR演算を用いて引数に指定する
	 *	  FOREGROUND_BLUE      0x0001 // text color contains blue.
	 *	  FOREGROUND_GREEN     0x0002 // text color contains green.
	 *	  FOREGROUND_RED       0x0004 // text color contains red.
	 *	  FOREGROUND_INTENSITY 0x0008 // text color is intensified.
	 *	  BACKGROUND_BLUE      0x0010 // background color contains blue.
	 *	  BACKGROUND_GREEN     0x0020 // background color contains green.
	 *	  BACKGROUND_RED       0x0040 // background color contains red.
	 *	  BACKGROUND_INTENSITY 0x0080 // background color is intensified.
	 *	  COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
	 *	  COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
	 *	  COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
	 *	  COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
	 *	  COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
	 *	  COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
	 *	  COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
	 *	  COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.
	 */
	void SetTextAttribute(int attribute)
	{
		SetConsoleTextAttribute(display_handle[swap_flg], attribute);
	}

	/**
	 * @brief	カーソルタイプ設定
	 *
	 * @param	type [入力] NOCURSOR カーソル表示なし
	 *                      SOLIDCURSOR (非対応)
	 *                      NORMALCURSOR カーソルの通常表示
	 */
	void SetCursorType(int type)
	{
		CONSOLE_CURSOR_INFO	cursor_info;
		int size = -1;

		if (size < 0) {
			if (GetConsoleCursorInfo(display_handle[swap_flg], &cursor_info)) {
				size = (int)cursor_info.dwSize;
			}
			else {
				size = 25;
			}
		}
		cursor_info.dwSize = (type < NORMALCURSOR) ? 100 : size;
		cursor_info.bVisible = (type != NOCURSOR);
		SetConsoleCursorInfo(display_handle[swap_flg], &cursor_info);
	}

	/**
	 * @brief	現在行に挿入
	 */
	void InsertLine(void)
	{
		COORD	lc{};
		DWORD	len;
		DWORD	num;
		LPTSTR	psz;
		LPWORD	pw;

		lc.X = screen_buffer_info.srWindow.Left;
		len = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1;
		psz = (LPTSTR)_malloca(len * sizeof(TCHAR));
		pw = (LPWORD)_malloca(len * sizeof(WORD));
		if (psz == NULL) {
			return;
		}
		if (pw == NULL) {
			return;
		}
		for (lc.Y = screen_buffer_info.srWindow.Bottom; lc.Y > screen_buffer_info.dwCursorPosition.Y; lc.Y--) {
			lc.Y--;
			ReadConsoleOutputAttribute(display_handle[swap_flg], pw, len, lc, &num);
			ReadConsoleOutputCharacter(display_handle[swap_flg], psz, len, lc, &num);
			lc.Y++;
			WriteConsoleOutputAttribute(display_handle[swap_flg], pw, len, lc, &num);
			WriteConsoleOutputCharacter(display_handle[swap_flg], psz, len, lc, &num);
		}
		FillConsoleOutputAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes, len, lc, &num);
		FillConsoleOutputCharacter(display_handle[swap_flg], TEXT(' '), len, lc, &num);
		_freea(psz);
		_freea(pw);
	}

	/**
	 * @brief	現在行の削除
	 */
	void DeleteLine(void)
	{
		DWORD	read_num;
		DWORD	write_num;
		DWORD	fill_num;
		COORD	calc_coord{};
		DWORD	line_len;
		LPTSTR	receive_character;
		LPWORD	receive_attribute;

		calc_coord.X = screen_buffer_info.srWindow.Left;
		line_len = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1;
		receive_character = (LPTSTR)_malloca(line_len * sizeof(TCHAR));
		receive_attribute = (LPWORD)_malloca(line_len * sizeof(WORD));
		if (receive_character == NULL) {
			return;
		}
		if (receive_attribute == NULL) {
			return;
		}

		for (calc_coord.Y = screen_buffer_info.dwCursorPosition.Y; calc_coord.Y < screen_buffer_info.srWindow.Bottom; calc_coord.Y++) {
			calc_coord.Y++;
			ReadConsoleOutputAttribute(display_handle[swap_flg], receive_attribute, line_len, calc_coord, &read_num);
			ReadConsoleOutputCharacter(display_handle[swap_flg], receive_character, line_len, calc_coord, &read_num);
			calc_coord.Y--;
			WriteConsoleOutputAttribute(display_handle[swap_flg], receive_attribute, line_len, calc_coord, &write_num);
			WriteConsoleOutputCharacter(display_handle[swap_flg], receive_character, line_len, calc_coord, &write_num);
		}

		//FillConsoleOutputAttribute(display_handle[swap_flg], screen_buffer_attribute, line_len, calc_coord, &fill_num);
		FillConsoleOutputCharacter(display_handle[swap_flg], TEXT(' '), line_len, calc_coord, &fill_num);

		_freea(receive_character);
		_freea(receive_attribute);
	}

	/**
	 * @brief	文字列の一括転送
	 *
	 * @param	buf [入力] スクリーンバッファのポインタ
	 */
	void PrintFrameBuffer(char* buf)
	{
		CHAR_INFO* char_info = screen_buffer;

		// 画像描画(画面外へのはみ出し処理なし)
		for (int count = 0; count < screen_buffer_size.Y * screen_buffer_size.X; count++) {
			// 背景色を4ビット左シフトしてセット
			char_info->Attributes = (*buf & 0x0F) | ((*buf << 4) & 0xF0);
			char_info++;
			buf++;
		}

		// 画面の一括描画
		WriteConsoleOutputA(display_handle[swap_flg], screen_buffer, screen_buffer_size, { 0,0 }, &window_size);
	}

	/**
	 * @brief	一括転送用バッファクリア
	 *
	 * @param	buf [入力] スクリーンバッファのポインタ
	 * @note
	 *  スクリーンバッファはウィンドウサイズの横幅×縦幅の
	 *  バイトサイズ以上のchar型配列とする
	 *  スクリーンバッファの内容を全て0でクリアする
	 */
	void ClearFrameBuffer(char* buf)
	{
		ZeroMemory(buf, sizeof(char) * screen_buffer_size.X * screen_buffer_size.Y);
	}

	/**
	 * @brief	コンソールウィンドウのタイトルバーに表示されるテキストを設定
	 *
	 * @param	title [入力] ウィンドウタイトルに表示するテキスト
	 */
	void SetCaption(const char* title)
	{
		SetConsoleTitleA(title);
	}

	/**
	 * @brief	コンソールウィンドウのタイトルバーに表示されるテキストを取得
	 *
	 * @param	title [出力] 現在のウィンドウタイトルのテキスト
	 * @param	len [入力] ウィンドウタイトルの文字数
	 *
	 * @retval	非0	現在のウィンドウタイトルの文字数
	 * @retval	0	エラー
	 */
	int GetCaption(char* title, int len)
	{
		return GetConsoleTitleA(title, len);
	}

	/**
	 * @brief	キー情報リセット
	 */
	void ResetKeyMap(void)
	{
		for (int count = 0; count < 8; count++) {
			conio_key_map[count] = 0;
		}
	}

	/**
	 * @brief	キーボード・マウス入力
	 *
	 * @param	port [入力] ポート番号(P*_*)
	 * @return	入力値
	 */
	int InputKeyMouse(int port)
	{
		DWORD event = 0;
		DWORD read = 0;
		PINPUT_RECORD input_record;
		KEY_EVENT_RECORD* key_event;
		MOUSE_EVENT_RECORD* mouse_event;

		// キーボードイベントチェック
		if (GetNumberOfConsoleInputEvents(input_handle, &event) && event) {
			read = 0;
			input_record = (PINPUT_RECORD)_malloca(event * sizeof(INPUT_RECORD));
			if (input_record == NULL) {
				return NULL;
			}
			PINPUT_RECORD allocated = input_record;
			if (ReadConsoleInput(input_handle, input_record, event, &read) && read) {
				input_record = input_record;
				for (unsigned int count = 0; count < read; count++, input_record++) {
					switch (input_record->EventType) {
					case KEY_EVENT: {
						key_event = &input_record->Event.KeyEvent;
						if (key_event->wVirtualKeyCode > 0x0FF) {
							break;
						}
						if (key_event->bKeyDown) {
							conio_key_map[key_event->wVirtualKeyCode >> 5] |= (0x01 << (key_event->wVirtualKeyCode & 31));
						}
						else {
							conio_key_map[key_event->wVirtualKeyCode >> 5] &= ~(0x01 << (key_event->wVirtualKeyCode & 31));
						}
						if (key_event->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
							conio_key_map[VK_MENU >> 5] |= (0x01 << (VK_MENU & 31));
						}
						else {
							conio_key_map[VK_MENU >> 5] &= ~(0x01 << (VK_MENU & 31));
						}
						if (key_event->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
							conio_key_map[VK_CONTROL >> 5] |= (0x01 << (VK_CONTROL & 31));
						}
						else {
							conio_key_map[VK_CONTROL >> 5] &= ~(0x01 << (VK_CONTROL & 31));
						}
						if (key_event->dwControlKeyState & SHIFT_PRESSED) {
							conio_key_map[VK_SHIFT >> 5] |= (0x01 << (VK_SHIFT & 31));
						}
						else {
							conio_key_map[VK_SHIFT >> 5] &= ~(0x01 << (VK_SHIFT & 31));
						}
						break;
					}
					case MOUSE_EVENT: {
						mouse_event = &input_record->Event.MouseEvent;
						conio_mouse_position = mouse_event->dwMousePosition;
						if (mouse_event->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
							conio_key_map[VK_LBUTTON >> 5] |= (0x01 << (VK_LBUTTON & 31));
						}
						else {
							conio_key_map[VK_LBUTTON >> 5] &= ~(0x01 << (VK_LBUTTON & 31));
						}
						if (mouse_event->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
							conio_key_map[VK_MBUTTON >> 5] |= (0x01 << (VK_MBUTTON & 31));
						}
						else {
							conio_key_map[VK_MBUTTON >> 5] &= ~(0x01 << (VK_MBUTTON & 31));
						}
						if (mouse_event->dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
							conio_key_map[VK_RBUTTON >> 5] |= (0x01 << (VK_RBUTTON & 31));
						}
						else {
							conio_key_map[VK_RBUTTON >> 5] &= ~(0x01 << (VK_RBUTTON & 31));
						}
						break;
					}
					default:
						break;
					}
				}
			}
			_freea(allocated);
		}

		// マウス座標を返す
		switch (port) {
		case PM_CURX:
			return conio_mouse_position.X + 1;
		case PM_CURY:
			return conio_mouse_position.Y + 1;
		default:
			break;
		}
		// キー状態を返す
		return (conio_key_map[(port & 0x0FF) >> 5] & (0x01 << (port & 31))) != 0;
	}

	/**
	 * @brief	ジョイパッド入力
	 *
	 * @param	port [入力] ポート番号(P*_*)
	 * @return	入力値
	 */
	int InputJoystick(int port)
	{
		JOYINFO	joy_info;
		int id;
		int func;

		// ゲームパッド入力
		if ((port & 0xfe00) == 0x0200) {
			id = (port & 0x01f0) >> 4;
			func = port & 0x0f;

			switch (func) {
			case 0:
			case 1:
			case 2:
			case 3:
				if (joyGetPos(id, &joy_info) != JOYERR_NOERROR) {
					return -1;
				}
				switch (func) {
				case 0:
					return joy_info.wXpos;
				case 1:
					return joy_info.wYpos;
				case 2:
					return joy_info.wZpos;
				case 3:
					return joy_info.wButtons;
				}
				break;
			default:
				break;
			}
			return 0;
		}
		return -1;
	}

	/**
	 * @brief	ジョイパッド入力(XInput対応)
	 *
	 * @param	port [入力] ポート番号(P*_*)
	 * @return	入力値
	 */
	int InputJoystickX(int port)
	{
		XINPUT_STATE controller_state[4]{};	// XInputコントローラ情報
		int id;
		int func;
		unsigned int  result;

		// ゲームパッド入力
		if ((port & 0xfe00) == 0x0200) {
			id = (port & 0x01f0) >> 4;
			func = port & 0x0f;

			// Simply get the state of the controller from XInput.
			result = XInputGetState(id, &controller_state[id]);
			if (result == ERROR_SUCCESS) {
				switch (func) {
				case 0:
					return controller_state[id].Gamepad.sThumbLX;
				case 1:
					return controller_state[id].Gamepad.sThumbLY;
				case 2:
					return controller_state[id].Gamepad.bLeftTrigger;
				case 3:
					return controller_state[id].Gamepad.sThumbRX;
				case 4:
					return controller_state[id].Gamepad.sThumbRY;
				case 5:
					return controller_state[id].Gamepad.bRightTrigger;
				case 6:
					return controller_state[id].Gamepad.wButtons;
				}
			}
		}
		return -1;
	}

	/**
	 * @brief	サウンド ファイルを開く
	 *
	 * @param	path [入力] ファイル名
	 * @retval	非0	サウンド ハンドル
	 * @retval	0	エラー
	 */
	int* MciOpenSound(const char* path)
	{
		union {
			MCI_WAVE_OPEN_PARMSA	wave_param;
			MCI_OPEN_PARMSA			open_param;
		} MciParam;
		TCHAR error_str[256];
		const char midi_ext[] = ".mid|.midi|.rmi";
		const char wave_ext[] = ".wav|.wave";
		const char mp3_ext[] = ".mp3";
		char input_ext[_MAX_EXT];
		DWORD_PTR mci_command;
		MCIERROR mci_error;
		MciSoundInfo* sound_info;

		sound_info = (MciSoundInfo*)malloc(sizeof(MciSoundInfo));
		if (sound_info == NULL) {
			return 0;
		}
		ZeroMemory(sound_info, sizeof(*sound_info));
		ZeroMemory(&MciParam, sizeof(MciParam));
		_splitpath_s(path, NULL, 0, NULL, 0, NULL, 0, input_ext, sizeof(input_ext));
		_strlwr_s(input_ext, strlen(input_ext) + 1);
		mci_command = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
		if (strstr(midi_ext, input_ext)) {
			sound_info->device_type = MCI_DEVTYPE_SEQUENCER;
			LPSTR dummy = lstrcpynA(sound_info->path, path, MAX_PATH);
			MciParam.open_param.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_SEQUENCER;
			MciParam.open_param.lpstrElementName = sound_info->path;
			mci_command |= MCI_OPEN_TYPE_ID;
		}
		else if (strstr(wave_ext, input_ext)) {
			sound_info->device_type = MCI_DEVTYPE_WAVEFORM_AUDIO;
			LPSTR dummy = lstrcpynA(sound_info->path, path, MAX_PATH);
			MciParam.wave_param.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
			MciParam.wave_param.lpstrElementName = sound_info->path;
			mci_command |= MCI_OPEN_TYPE_ID;
			// MciParam.wave_param.dwBufferSeconds  = 60;
			// mci_command |= MCI_WAVE_OPEN_BUFFER;
		}
		else if (strstr(mp3_ext, input_ext)) {
			sound_info->device_type = MCI_DEVTYPE_DIGITAL_VIDEO;
			LPSTR dummy = lstrcpynA(sound_info->path, path, MAX_PATH);
			MciParam.open_param.lpstrDeviceType = "MPEGVideo";
			MciParam.open_param.lpstrElementName = sound_info->path;
		}
		else {
			free(sound_info);
			return 0;
		}
		mci_error = mciSendCommandA(0, MCI_OPEN, mci_command, (DWORD_PTR)&MciParam);
		if (mci_error != 0) {
			free(sound_info);
			mciGetErrorString(mci_error, error_str, sizeof(error_str) / sizeof(TCHAR));
			MessageBox(NULL, error_str, NULL, MB_ICONWARNING);
			return 0;
		}
		sound_info->device_id = MciParam.open_param.wDeviceID;
		return (int*)sound_info;
	}

	/**
	 * @brief	サウンド ファイルを閉じる
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 */
	void MciCloseSound(int* sound_id)
	{
		MciSoundInfo* sound_info;

		if (!sound_id) {
			return;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (sound_info->device_id) {
			mciSendCommand(sound_info->device_id, MCI_CLOSE, 0, 0);
			sound_info->device_id = 0;
		}
		free(sound_info);
	}

	/**
	 * @brief	サウンドを再生する
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 * @param	repeat [入力] ループ有無
	 */
	void MciPlaySound(int* sound_id, int repeat)
	{
		MciSoundInfo* sound_info;
		DWORD_PTR mci_command;
		MCI_PLAY_PARMS play_param;

		if (!sound_id) {
			return;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (!sound_info->device_id) {
			return;
		}
		sound_info->repeat = repeat;
		ZeroMemory(&play_param, sizeof(play_param));
		mci_command = 0;
		if (repeat) {
			switch (sound_info->device_type) {
			case MCI_DEVTYPE_DIGITAL_VIDEO:
				mci_command |= (MCI_FROM | MCI_DGV_PLAY_REPEAT);
				play_param.dwFrom = 0;
				break;
			case MCI_DEVTYPE_SEQUENCER:
			case MCI_DEVTYPE_WAVEFORM_AUDIO:
				break;
			default:
				break;
			}
		}
		mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
		mciSendCommand(sound_info->device_id, MCI_PLAY, mci_command, (DWORD_PTR)&play_param);
	}

	/**
	 * @brief	サウンド再生を停止する
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 */
	void MciStopSound(int* sound_id)
	{
		MciSoundInfo* sound_info;

		if (!sound_id) {
			return;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (!sound_info->device_id) {
			return;
		}
		sound_info->repeat = 0;
		mciSendCommand(sound_info->device_id, MCI_STOP, MCI_WAIT, 0);
		mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
	}

	/**
	 * @brief	サウンド再生状態の取得
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 * @return	再生中ならば 0 以外を返す
	 */
	int MciCheckSound(int* sound_id)
	{
		MciSoundInfo* sound_info;
		MCI_STATUS_PARMS status_param;

		if (!sound_id) {
			return 0;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (!sound_info->device_id) {
			return 0;
		}
		ZeroMemory(&status_param, sizeof(status_param));
		status_param.dwItem = MCI_STATUS_MODE;
		if (mciSendCommand(sound_info->device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status_param)) {
			return 0;
		}
		return status_param.dwReturn == MCI_MODE_PLAY;
	}

	/**
	 * @brief	ループ再生の強制更新
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 * @note
	 *  サウンドが停止したら同じサウンドを再生する
	 *  更新時は音量設定が標準値に戻るので再設定を行う必要がある
	 */
	void MciUpdateSound(int* sound_id)
	{
		MciSoundInfo* sound_info;
		MCI_STATUS_PARMS status_param;

		if (!sound_id) {
			return;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (!sound_info->device_id || !sound_info->repeat) {
			return;
		}

		switch (sound_info->device_type) {
		case MCI_DEVTYPE_DIGITAL_VIDEO:
			break;
		case MCI_DEVTYPE_SEQUENCER:
		case MCI_DEVTYPE_WAVEFORM_AUDIO:
			ZeroMemory(&status_param, sizeof(status_param));
			status_param.dwItem = MCI_STATUS_MODE;
			if (!mciSendCommand(sound_info->device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status_param)) {
				if (status_param.dwReturn == MCI_MODE_NOT_READY) {
				}
				else if (status_param.dwReturn == MCI_MODE_STOP) {
					if (sound_info->repeat > 0) {
						mciSendCommand(sound_info->device_id, MCI_SEEK, MCI_SEEK_TO_START, 0);
						mciSendCommand(sound_info->device_id, MCI_PLAY, 0, 0);
					}
				}
			}
			break;
		default:
			break;
		}
	}


	/**
	 * @brief	再生音量を設定する
	 *
	 * @param	sound_id [入力] サウンド ハンドル
	 * @param	percent [入力] 音量 (0 ～ 100)
	 */
	void MciSetVolume(int* sound_id, int percent)
	{
		MciSoundInfo* sound_info;
		MCI_DGV_SETAUDIO_PARMS	audio_param;
		DWORD volume;

		if (!sound_id) {
			return;
		}
		sound_info = (MciSoundInfo*)sound_id;
		if (!sound_info->device_id) {
			return;
		}
		switch (sound_info->device_type) {
		case MCI_DEVTYPE_DIGITAL_VIDEO:
			ZeroMemory(&audio_param, sizeof(audio_param));
			audio_param.dwItem = MCI_DGV_SETAUDIO_VOLUME;
			audio_param.dwValue = percent * 10;
			mciSendCommand(sound_info->device_id, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD_PTR)&audio_param);
			break;
		case MCI_DEVTYPE_SEQUENCER:
			volume = 0x0ffff * percent / 100;
			midiOutSetVolume(0, (DWORD)MAKELONG(volume, volume));
			break;
		case MCI_DEVTYPE_WAVEFORM_AUDIO:
			volume = 0x0ffff * percent / 100;
			waveOutSetVolume(0, (DWORD)MAKELONG(volume, volume));
			break;
		default:
			break;
		}
	}

	// ==== 拡張文字列処理 ====
	/**
	* 半角文字を全角文字に変換する
	* _src:変換元になる文字列
	* return:変換後の文字列が入っているバッファへのポインタ。
	* 【注】戻り値にmalloc()したポインタを返すので、呼び出した側で必ずfree()する事。
	*/
	wchar_t* HanToZenW(const wchar_t* _src)
	{
		//---- Unicode文字列の半角文字を全角文字に変換する ----
		DWORD flags = LCMAP_FULLWIDTH;      //全角文字に変換
		//	DWORD flags = LCMAP_HALFWIDTH;      //半角文字に変換
		//  DWORD flags = LCMAP_HIRAGANA;       //ひらがなに変換
		//  DWORD flags = LCMAP_KATAKANA;       //カタカナに変換
		int dest_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, NULL, 0, NULL, NULL, 0);
		//変換先バッファを確保。
		wchar_t* dest_buf = (wchar_t*)malloc(dest_size * sizeof(wchar_t));
		if (dest_buf == NULL) {
			return NULL;
		}
		memset(dest_buf, 0, dest_size * sizeof(wchar_t));	//変換先バッファを０で初期化
		int output_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, dest_buf, dest_size, NULL, NULL, 0);
		return dest_buf;
	}
	/**
	* 半角文字を全角文字に変換する
	* _src:変換元になる文字列
	* return:変換後の文字列が入っているバッファへのポインタ。
	* 【注】戻り値にmalloc()したポインタを返すので、呼び出した側で必ずfree()する事。
	*/
	char* HanToZenA(const char* _src)
	{
		//const char* _src = "変換するabcxyz;*@文字列1234567890";
		//---- マルチバイト文字列をUnicode文字列に変換する
		int wc_count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);	//'\0'を含む文字数が返る
		size_t wc_src_bytes = (wc_count * sizeof(wchar_t));
		wchar_t* src_txt = (wchar_t*)_malloca(wc_src_bytes);
		memset(src_txt, 0, wc_src_bytes);
		int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, src_txt, wc_count);
		//---- Unicode文字列の半角文字を全角文字に変換する ----
		DWORD flags = LCMAP_FULLWIDTH;      //全角文字に変換
		//	DWORD flags = LCMAP_HALFWIDTH;      //半角文字に変換
		//  DWORD flags = LCMAP_HIRAGANA;       //ひらがなに変換
		//  DWORD flags = LCMAP_KATAKANA;       //カタカナに変換
		int dest_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, src_txt, -1, NULL, 0, NULL, NULL, 0);
		wchar_t* dest_buf = (wchar_t*)_malloca(dest_size * sizeof(wchar_t));
		if (dest_buf == NULL) {
			return NULL;
		}
		if (dest_buf == NULL) {
			return NULL;
		}
		memset(dest_buf, 0, dest_size * sizeof(wchar_t));
		int output_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, src_txt, -1, dest_buf, dest_size, NULL, NULL, 0);
		//---- Unicode文字列をマルチバイト文字列に変換する ----
		int mb_bytes = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, NULL, 0, NULL, NULL);	//'\0'含むサイズが戻る
		//変換先バッファを確保。
		char* mb_dest_buff = (char*)malloc(mb_bytes);
		if (mb_dest_buff == NULL) {
			return NULL;
		}
		memset(mb_dest_buff, 0, mb_bytes);	//変換先バッファを０で初期化
		int res = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, mb_dest_buff, mb_bytes, NULL, NULL);
		return mb_dest_buff;
	}

	/**
	* 書式付きPrintFString
	* _zenkaku:trueなら全ての文字を全角で出力
	* 例）
	*	false:"全角%d",99 -> "全角99"
	*	true:"全角%d",99 -> "全角９９"
	* _format:書式指定文字列
	* ...：任意の可変長引数
	* ※_zenkaku以降はprintf()と同じ仕様。
	*/
	void PrintStringFA(bool _zenkaku, const char* _format, ...)
	{
		va_list ap;
		va_start(ap, _format);
		size_t length = (size_t)_vscprintf(_format, ap) + 1;	//'\0'含まないので＋１している
		char* buf = (char*)_malloca(length);
		if (buf == NULL) {
			return;
		}
		vsprintf_s(buf, length, _format, ap);
		if (_zenkaku == true) {
			char* p = HanToZen(buf);
			PrintString(p, -1);
			free(p);
		}
		else {
			PrintString(buf, -1);
		}
		va_end(ap);
	}
	void PrintStringFW(bool _zenkaku, const wchar_t* _format, ...)
	{
		va_list ap;
		va_start(ap, _format);
		size_t length = (size_t)_vscwprintf(_format, ap) + 1;	//'\0'含まないので＋１している
		wchar_t* buf = (wchar_t*)_malloca(length*sizeof(wchar_t));
		if (buf == NULL) {
			return;
		}
		vswprintf_s(buf, length, _format, ap);
		if (_zenkaku == true) {
			wchar_t* p = HanToZenW(buf);
			PrintStringW(p, -1);
			free(p);
		}
		else {
			PrintStringW(buf, -1);
		}
		va_end(ap);
	}

#ifdef __cplusplus
}
#endif

/**
* @copyright (c) 2018-2019 HAL Osaka College of Technology & Design (Ihara, H.)
* (2023/11 Adjusted by Tatsumi, K.)
*/
