/**
* @file	conioex.h
* @brief	Win32 �R���\�[�� I/O �g�� for Visual C++/Borland C++
*
* @date	2023/11 2023�N�x��
* @version	1.02
* @note
*  �R���\�[���E�B���h�E�̃v���p�e�B�̐ݒ���ȉ��ɕύX���邱��
*  �E�u�]���̃R���\�[�����g���v�̃`�F�b�N���O��
*  �E�u�ȈՕҏW���[�h�v�̃`�F�b�N���O��
*
* �R���\�[���֐��ɂ���
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
	int cursor_origin = 1;	// �E�B���h�E����[�̃J�[�\�����W�B(1, 1)�܂���(0, 0)

	CHAR_INFO* screen_buffer = NULL;

	/**
	 * @brief	�R���\�[�� I/O ������
	 *
	 * @param	width [����] �R���\�[���E�B���h�E�̉��T�C�Y(60�`)
	 * @param	height [����] �R���\�[���E�B���h�E�̏c�T�C�Y(1�`)
	 */
	COORD InitConio(int width, int height)
	{
		// �E�B���h�E�T�C�Y���Œ�l�����Ȃ�Œ�l�ɕύX
		if (height < CONSOLE_WINDOW_HEIGHT_MIN) {
			height = CONSOLE_WINDOW_HEIGHT_MIN;
		}
		if (width < CONSOLE_WINDOW_WIDTH_MIN) {
			width = CONSOLE_WINDOW_WIDTH_MIN;
		}

		CONSOLE_HISTORY_INFO history_info{};

		// �f�B�X�v���C���擾
		display_handle[0] = GetStdHandle(STD_OUTPUT_HANDLE);
		display_handle[1] = NULL;
		input_handle = GetStdHandle(STD_INPUT_HANDLE);

		// �R�}���h������ۑ����Ȃ�
		history_info.cbSize = sizeof(CONSOLE_HISTORY_INFO);
		history_info.HistoryBufferSize = 0;
		history_info.NumberOfHistoryBuffers = 0;
		history_info.dwFlags = 0;
		if (SetConsoleHistoryInfo(&history_info) == 0) {
			printf("Failed SetConsoleHistoryInfo ErrorCode:%d\n", GetLastError());
		}

		// �ꎞ�I�Ƀt�H���g�T�C�Y���ŏ��l�ɐݒ�
		font_size.dwFontSize.X = 1;
		font_size.dwFontSize.Y = 1;
		if (SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size) == 0) {
			printf("Failed SetCurrentConsoleFontEx ErrorCode:%d\n", GetLastError());
		}

		// ���݂̃t�H���g�T�C�Y�ƃf�B�X�v���C�̉𑜓x����A�ő�E�B���h�E�T�C�Y���v�Z
		COORD maxSize = GetLargestConsoleWindowSize(display_handle[0]);
		if (width > maxSize.X) {
			width = maxSize.X;
		}
		if (height > maxSize.Y) {
			height = maxSize.Y;
		}

		// �E�B���h�E�T�C�Y�ύX
		window_size.Left = 0;
		window_size.Top = 0;
		window_size.Right = width - 1;
		window_size.Bottom = height - 1;
		if (SetConsoleWindowInfo(display_handle[0], TRUE, &window_size) == 0) {
			printf("Failed SetConsoleWindowInfo ErrorCode:%d\n", GetLastError());
		}

		// �o�b�t�@�T�C�Y�ύX
		screen_buffer_size.X = width;
		screen_buffer_size.Y = height;
		if (SetConsoleScreenBufferSize(display_handle[0], screen_buffer_size) == 0) {
			printf("Failed SetConsoleScreenBufferSize ErrorCode:%d\n", GetLastError());
		}

		// �E�B���h�E�T�C�Y�ĕύX
		window_size.Left = 0;
		window_size.Top = 0;
		window_size.Right = width - 1;
		window_size.Bottom = height - 1;
		if (SetConsoleWindowInfo(display_handle[0], TRUE, &window_size) == 0) {
			printf("Failed SetConsoleWindowInfo ErrorCode:%d\n", GetLastError());
		}

		// �R���\�[���E�B���h�E�̃��[�h�ݒ�
		if (SetConsoleMode(display_handle[0], CONSOLE_OUTPUT_MODE) == 0) {
			printf("Failed SetConsoleMode ErrorCode:%d\n", GetLastError());
		}
		if (SetConsoleMode(input_handle, CONSOLE_INPUT_MODE) == 0) {
			printf("Failed SetConsoleMode ErrorCode:%d\n", GetLastError());
		}

		// �X�N���[���o�b�t�@�̏����擾
		if (GetConsoleScreenBufferInfo(display_handle[0], &screen_buffer_info) == 0) {
			printf("Failed GetConsoleScreenBufferInfo ErrorCode:%d\n", GetLastError());
		}

		// �X�N���[���o�b�t�@�p�̔z����쐬
		screen_buffer = new CHAR_INFO[width * height];
		memset(screen_buffer, 0, (size_t)width * height);

		// �t�H���g�T�C�Y���Đݒ�
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
	 * @brief	�_�u���o�b�t�@������
	 *
	 * @return	�o�b�t�@�n���h���擾���s
	 */
	int InitDoubleBuffer(void)
	{
		// �_�u���o�b�t�@�p�̃������[���m��
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

		// �t�H���g�T�C�Y�̕ύX
		SetCurrentConsoleFontEx(display_handle[0], TRUE, &font_size);
		SetCurrentConsoleFontEx(display_handle[1], TRUE, &font_size);

		SetConsoleScreenBufferSize(display_handle[0], screen_buffer_size);	// �o�b�t�@�T�C�Y�ύX
		SetConsoleWindowInfo(display_handle[0], TRUE, &window_size);		// �E�B���h�E�T�C�Y�ύX

		SetConsoleScreenBufferSize(display_handle[1], screen_buffer_size);	// �o�b�t�@�T�C�Y�ύX
		SetConsoleWindowInfo(display_handle[1], TRUE, &window_size);		// �E�B���h�E�T�C�Y�ύX

		SetConsoleMode(display_handle[0], CONSOLE_OUTPUT_MODE);	// �o�b�t�@���㏑�����[�h��
		SetConsoleMode(display_handle[1], CONSOLE_OUTPUT_MODE);	// �o�b�t�@���㏑�����[�h��

		return 0;
	}

	/**
	 * @brief	�t�H���g�T�C�Y�ύX
	 *
	 * @param	width [����] �t�H���g�̉��T�C�Y(1�`)
	 * @param	height [����] �t�H���g�̏c�T�C�Y(1�`)
	 */
	void SetScreenFontSize(int width, int height)
	{
		// �t�H���g�T�C�Y�ύX
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
	 * @brief	���������̃J�[�\���ʒu���擾
	 *
	 * @return	���݂̃J�[�\���ʒu��X���W(1�`)�܂���(0�`)
	 */
	int GetCursorX(void)
	{
		return screen_buffer_info.dwCursorPosition.X - screen_buffer_info.srWindow.Left - cursor_origin;
	}

	/**
	 * @brief	���������̃J�[�\���ʒu���擾
	 *
	 * @return	���݂̃J�[�\���ʒu��Y���W(1�`)�܂���(0�`)
	 */
	int GetCursorY(void)
	{
		return screen_buffer_info.dwCursorPosition.Y - screen_buffer_info.srWindow.Top - cursor_origin;
	}

	/**
	 * @brief	�J�[�\���ʒu�̈ړ�
	 *
	 * @param	x [����] X���W(1�`)�܂���(0�`)
	 * @param	y [����] Y���W(1�`)�܂���(0�`)
	 */
	void SetCursorPosition(int x, int y)
	{
		COORD lc{};

		lc.X = x - cursor_origin;
		lc.Y = screen_buffer_info.srWindow.Top + y - cursor_origin;

		SetConsoleCursorPosition(display_handle[swap_flg], lc);
	}

	/**
	 * @brief	���������̃X�N���[���T�C�Y���擾
	 *
	 * @return	���������̃X�N���[���T�C�Y
	 */
	int GetScreenSizeX(void) {
		return screen_buffer_size.X;
	}

	/**
	 * @brief	���������̃X�N���[���T�C�Y���擾
	 *
	 * @return	���������̃X�N���[���T�C�Y
	 */
	int GetScreenSizeY(void) {
		return screen_buffer_size.Y;
	}

	/**
	 * @brief	�J�[�\���ʒu�̌��_��ݒ�
	 *
	 * @param	flag [����] true�Ȃ猴�_��0�ɂ���Afalse�Ȃ猴�_��1�ɂ���
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
	 * @brief	������̏o��(�}���`�o�C�g��Unicode�ϊ���)
	 *
	 * @param	_src [����] �o�͕�����z��̃|�C���^
	 * @param	_size [����] �o�͕�����
	 */
	void PrintString(const char* _src, int _size)
	{
		//�w�肳�ꂽ�}���`�o�C�g�����S�Ă����C�h����(Unicode����)�ϊ������ꍇ��
		//�K�v�ȃo�b�t�@�[�T�C�Y (�I�[�� null �������܂�) �𕶎��P�ʂŎZ�o���A
		//���̕��������̃o�b�t�@�[���m�ۂ���B
		int wc_src_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);
		size_t wc_src_bytes = (wc_src_siz * sizeof(wchar_t));	//Unicode�������Z�̃o�C�g���B
		wchar_t* wc_src = (wchar_t*)_malloca(wc_src_bytes);	//�X�^�b�N��Ɋm�ہFfree�s�v
		if (wc_src == NULL) {
			return;
		}
		memset(wc_src, 0, wc_src_bytes);	//�O�N���A�F�������݂��r���܂łł�'\0'�I�[������ɂȂ�B
		//�w��T�C�Y�����̕��������I�[�o�[���Ă���ꍇ�̕␳
		if (strlen(_src) < _size) {
			_size = (-1);	//(-1)�w���'\0'�܂ŕϊ��B
		}
		//�w��T�C�Y���ϊ�����i_size == (-1))�Ȃ�'\0'�܂őS�ĕϊ�����j
		//�߂�l�͕ϊ�����(�o�b�t�@�ɏ������܂ꂽ)���������Ԃ�B
		//�y���z(-1)�w��ŕϊ������ꍇ�A�߂�l��'\0'���܂ޕ������ɂȂ�B
		int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, _size, wc_src, wc_src_siz);
		//disp_siz��'\0'���܂ޕ�������������Ȃ��̂ŁA�ϊ������������ɂȂ�l�ɍČv�Z����B
		disp_siz = (int)wcslen(wc_src);
		DWORD num;	//���ۂɏ������܂ꂽ���������󂯎��ϐ�
		WriteConsoleW(display_handle[swap_flg], wc_src, disp_siz, &num, NULL);
		//WriteConsoleW(display_handle[swap_flg], pWChar, ret_val, &num, NULL);
		//WCHAR wide_char[256];
		//memset(wide_char, 0, sizeof(wide_char));
		//ret_val = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, srcbuf, size, wide_char, size);
		//WriteConsole(display_handle[swap_flg], wide_char, ret_val, &num, NULL);
	}
#endif	//UNICODE
	/**
	 * @brief	������̏o�́i�}���`�o�C�g�����p�j
	 *
	 * @param	_srcbuf [����] �o�͕�����z��̃|�C���^
	 * @param	_size [����] �o�͕�����
	 */
	void PrintStringA(const char* _srcbuf, int _size) {
		//�\�����������w�蕶����̃T�C�Y���I�[�o�[���Ă�����␳����
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
	 * @brief	������̏o�́iUnicode�����p�j
	 *
	 * @param	_srcbuf [����] �o�͕�����z��̃|�C���^
	 * @param	_size [����] �o�͕�����
	 */
	void PrintStringW(const wchar_t* _srcbuf, int _size) {
		//�\�����������w�蕶����̃T�C�Y���I�[�o�[���Ă�����␳����
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
	 * @brief	�s���܂ŏ���
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
	 * @brief	��ʏ���
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
	 * @brief	�_�u���o�b�t�@���̕`��ʐؑ�
	 */
	void FlipScreen(void)
	{
		SetConsoleActiveScreenBuffer(display_handle[swap_flg]);	// �o�b�t�@�����ւ��\��
		swap_flg = (swap_flg) ? 0 : 1;
	}

	/**
	 * @brief	�����F���P�x��
	 */
	void SetHighVideoColor(void)
	{
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes |= FOREGROUND_INTENSITY);
	}

	/**
	 * @brief	�����F��P�x��
	 */
	void SetLowVideoColor(void)
	{
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes &= ~FOREGROUND_INTENSITY);
	}

	/**
	 * @brief	���蕶���F�ݒ�
	 */
	void SetNormalVideoColor(void)
	{
		//SetConsoleTextAttribute(display_handle[swap_flg], LIGHTGRAY);
	}

	/**
	 * @brief	�����w�i�F�ݒ�
	 *
	 * @param	color [����] �����w�i�F
	 * @note
	 *  �w�i�F��enum COLORS���Q�Ƃ���
	 */
	void SetTextBackColor(int color)
	{
		screen_buffer_info.wAttributes &= ~0x00f0;
		//screen_buffer_info.wAttributes |= ((color & 0x07) << 4);
		screen_buffer_info.wAttributes |= ((color & 0x0F) << 4);
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes);
	}

	/**
	 * @brief	�����F�ݒ�
	 *
	 * @param	color [����] �����F
	 * @note
	 *  �����F��enum COLORS���Q�Ƃ���
	 */
	void SetConsoleTextColor(int color)
	{
		screen_buffer_info.wAttributes &= ~0x000f;
		screen_buffer_info.wAttributes |= (color & 0x0f);
		SetConsoleTextAttribute(display_handle[swap_flg], screen_buffer_info.wAttributes);
	}

	/**
	 * @brief	�����F�w�i�F�����ݒ�
	 *
	 * @param	attribute [����] �����w�i���
	 * @note
	 *  �ȉ��̊e�ݒ�l���r�b�g����OR���Z��p���Ĉ����Ɏw�肷��
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
	 * @brief	�J�[�\���^�C�v�ݒ�
	 *
	 * @param	type [����] NOCURSOR �J�[�\���\���Ȃ�
	 *                      SOLIDCURSOR (��Ή�)
	 *                      NORMALCURSOR �J�[�\���̒ʏ�\��
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
	 * @brief	���ݍs�ɑ}��
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
	 * @brief	���ݍs�̍폜
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
	 * @brief	������̈ꊇ�]��
	 *
	 * @param	buf [����] �X�N���[���o�b�t�@�̃|�C���^
	 */
	void PrintFrameBuffer(char* buf)
	{
		CHAR_INFO* char_info = screen_buffer;

		// �摜�`��(��ʊO�ւ̂͂ݏo�������Ȃ�)
		for (int count = 0; count < screen_buffer_size.Y * screen_buffer_size.X; count++) {
			// �w�i�F��4�r�b�g���V�t�g���ăZ�b�g
			char_info->Attributes = (*buf & 0x0F) | ((*buf << 4) & 0xF0);
			char_info++;
			buf++;
		}

		// ��ʂ̈ꊇ�`��
		WriteConsoleOutputA(display_handle[swap_flg], screen_buffer, screen_buffer_size, { 0,0 }, &window_size);
	}

	/**
	 * @brief	�ꊇ�]���p�o�b�t�@�N���A
	 *
	 * @param	buf [����] �X�N���[���o�b�t�@�̃|�C���^
	 * @note
	 *  �X�N���[���o�b�t�@�̓E�B���h�E�T�C�Y�̉����~�c����
	 *  �o�C�g�T�C�Y�ȏ��char�^�z��Ƃ���
	 *  �X�N���[���o�b�t�@�̓��e��S��0�ŃN���A����
	 */
	void ClearFrameBuffer(char* buf)
	{
		ZeroMemory(buf, sizeof(char) * screen_buffer_size.X * screen_buffer_size.Y);
	}

	/**
	 * @brief	�R���\�[���E�B���h�E�̃^�C�g���o�[�ɕ\�������e�L�X�g��ݒ�
	 *
	 * @param	title [����] �E�B���h�E�^�C�g���ɕ\������e�L�X�g
	 */
	void SetCaption(const char* title)
	{
		SetConsoleTitleA(title);
	}

	/**
	 * @brief	�R���\�[���E�B���h�E�̃^�C�g���o�[�ɕ\�������e�L�X�g���擾
	 *
	 * @param	title [�o��] ���݂̃E�B���h�E�^�C�g���̃e�L�X�g
	 * @param	len [����] �E�B���h�E�^�C�g���̕�����
	 *
	 * @retval	��0	���݂̃E�B���h�E�^�C�g���̕�����
	 * @retval	0	�G���[
	 */
	int GetCaption(char* title, int len)
	{
		return GetConsoleTitleA(title, len);
	}

	/**
	 * @brief	�L�[��񃊃Z�b�g
	 */
	void ResetKeyMap(void)
	{
		for (int count = 0; count < 8; count++) {
			conio_key_map[count] = 0;
		}
	}

	/**
	 * @brief	�L�[�{�[�h�E�}�E�X����
	 *
	 * @param	port [����] �|�[�g�ԍ�(P*_*)
	 * @return	���͒l
	 */
	int InputKeyMouse(int port)
	{
		DWORD event = 0;
		DWORD read = 0;
		PINPUT_RECORD input_record;
		KEY_EVENT_RECORD* key_event;
		MOUSE_EVENT_RECORD* mouse_event;

		// �L�[�{�[�h�C�x���g�`�F�b�N
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

		// �}�E�X���W��Ԃ�
		switch (port) {
		case PM_CURX:
			return conio_mouse_position.X + 1;
		case PM_CURY:
			return conio_mouse_position.Y + 1;
		default:
			break;
		}
		// �L�[��Ԃ�Ԃ�
		return (conio_key_map[(port & 0x0FF) >> 5] & (0x01 << (port & 31))) != 0;
	}

	/**
	 * @brief	�W���C�p�b�h����
	 *
	 * @param	port [����] �|�[�g�ԍ�(P*_*)
	 * @return	���͒l
	 */
	int InputJoystick(int port)
	{
		JOYINFO	joy_info;
		int id;
		int func;

		// �Q�[���p�b�h����
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
	 * @brief	�W���C�p�b�h����(XInput�Ή�)
	 *
	 * @param	port [����] �|�[�g�ԍ�(P*_*)
	 * @return	���͒l
	 */
	int InputJoystickX(int port)
	{
		XINPUT_STATE controller_state[4]{};	// XInput�R���g���[�����
		int id;
		int func;
		unsigned int  result;

		// �Q�[���p�b�h����
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
	 * @brief	�T�E���h �t�@�C�����J��
	 *
	 * @param	path [����] �t�@�C����
	 * @retval	��0	�T�E���h �n���h��
	 * @retval	0	�G���[
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
	 * @brief	�T�E���h �t�@�C�������
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
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
	 * @brief	�T�E���h���Đ�����
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
	 * @param	repeat [����] ���[�v�L��
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
	 * @brief	�T�E���h�Đ����~����
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
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
	 * @brief	�T�E���h�Đ���Ԃ̎擾
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
	 * @return	�Đ����Ȃ�� 0 �ȊO��Ԃ�
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
	 * @brief	���[�v�Đ��̋����X�V
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
	 * @note
	 *  �T�E���h����~�����瓯���T�E���h���Đ�����
	 *  �X�V���͉��ʐݒ肪�W���l�ɖ߂�̂ōĐݒ���s���K�v������
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
	 * @brief	�Đ����ʂ�ݒ肷��
	 *
	 * @param	sound_id [����] �T�E���h �n���h��
	 * @param	percent [����] ���� (0 �` 100)
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

	// ==== �g�������񏈗� ====
	/**
	* ���p������S�p�����ɕϊ�����
	* _src:�ϊ����ɂȂ镶����
	* return:�ϊ���̕����񂪓����Ă���o�b�t�@�ւ̃|�C���^�B
	* �y���z�߂�l��malloc()�����|�C���^��Ԃ��̂ŁA�Ăяo�������ŕK��free()���鎖�B
	*/
	wchar_t* HanToZenW(const wchar_t* _src)
	{
		//---- Unicode������̔��p������S�p�����ɕϊ����� ----
		DWORD flags = LCMAP_FULLWIDTH;      //�S�p�����ɕϊ�
		//	DWORD flags = LCMAP_HALFWIDTH;      //���p�����ɕϊ�
		//  DWORD flags = LCMAP_HIRAGANA;       //�Ђ炪�Ȃɕϊ�
		//  DWORD flags = LCMAP_KATAKANA;       //�J�^�J�i�ɕϊ�
		int dest_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, NULL, 0, NULL, NULL, 0);
		//�ϊ���o�b�t�@���m�ہB
		wchar_t* dest_buf = (wchar_t*)malloc(dest_size * sizeof(wchar_t));
		if (dest_buf == NULL) {
			return NULL;
		}
		memset(dest_buf, 0, dest_size * sizeof(wchar_t));	//�ϊ���o�b�t�@���O�ŏ�����
		int output_size = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, flags, _src, -1, dest_buf, dest_size, NULL, NULL, 0);
		return dest_buf;
	}
	/**
	* ���p������S�p�����ɕϊ�����
	* _src:�ϊ����ɂȂ镶����
	* return:�ϊ���̕����񂪓����Ă���o�b�t�@�ւ̃|�C���^�B
	* �y���z�߂�l��malloc()�����|�C���^��Ԃ��̂ŁA�Ăяo�������ŕK��free()���鎖�B
	*/
	char* HanToZenA(const char* _src)
	{
		//const char* _src = "�ϊ�����abcxyz;*@������1234567890";
		//---- �}���`�o�C�g�������Unicode������ɕϊ�����
		int wc_count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, NULL, 0);	//'\0'���܂ޕ��������Ԃ�
		size_t wc_src_bytes = (wc_count * sizeof(wchar_t));
		wchar_t* src_txt = (wchar_t*)_malloca(wc_src_bytes);
		memset(src_txt, 0, wc_src_bytes);
		int disp_siz = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _src, -1, src_txt, wc_count);
		//---- Unicode������̔��p������S�p�����ɕϊ����� ----
		DWORD flags = LCMAP_FULLWIDTH;      //�S�p�����ɕϊ�
		//	DWORD flags = LCMAP_HALFWIDTH;      //���p�����ɕϊ�
		//  DWORD flags = LCMAP_HIRAGANA;       //�Ђ炪�Ȃɕϊ�
		//  DWORD flags = LCMAP_KATAKANA;       //�J�^�J�i�ɕϊ�
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
		//---- Unicode��������}���`�o�C�g������ɕϊ����� ----
		int mb_bytes = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, NULL, 0, NULL, NULL);	//'\0'�܂ރT�C�Y���߂�
		//�ϊ���o�b�t�@���m�ہB
		char* mb_dest_buff = (char*)malloc(mb_bytes);
		if (mb_dest_buff == NULL) {
			return NULL;
		}
		memset(mb_dest_buff, 0, mb_bytes);	//�ϊ���o�b�t�@���O�ŏ�����
		int res = WideCharToMultiByte(CP_ACP, 0, dest_buf, -1, mb_dest_buff, mb_bytes, NULL, NULL);
		return mb_dest_buff;
	}

	/**
	* �����t��PrintFString
	* _zenkaku:true�Ȃ�S�Ă̕�����S�p�ŏo��
	* ��j
	*	false:"�S�p%d",99 -> "�S�p99"
	*	true:"�S�p%d",99 -> "�S�p�X�X"
	* _format:�����w�蕶����
	* ...�F�C�ӂ̉ϒ�����
	* ��_zenkaku�ȍ~��printf()�Ɠ����d�l�B
	*/
	void PrintStringFA(bool _zenkaku, const char* _format, ...)
	{
		va_list ap;
		va_start(ap, _format);
		size_t length = (size_t)_vscprintf(_format, ap) + 1;	//'\0'�܂܂Ȃ��̂Ł{�P���Ă���
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
		size_t length = (size_t)_vscwprintf(_format, ap) + 1;	//'\0'�܂܂Ȃ��̂Ł{�P���Ă���
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
