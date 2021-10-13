//�w�b�_�[�t�@�C���ǂݍ���
#include"game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���


//�\���̂̒�`
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ(�p�X)
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;          //��
	int height;         //����

	BOOL IsDraw = FALSE;//�摜���`��ł���H

};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			//�摜�\����
	int speed = 1;		//�ړ����x
	RECT coll;			//�����蔻��̗̈�i�l�p�j
	

};

//����̍\����
struct MOVIE
{
	int handle = -1;		//����n���h��
	char path[255];			//����̃p�X

	int x;					//���ʒu
	int y;					//���ʒu
	int width;				//��
	int height;				//����

	int Volume = 255;		//�{�����[��(�ŏ��j�O�`255(�ő�j
};

struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//�{�����[���iMIN 0�`255 MAX)
	int playType = -1;
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//�摜��ǂݍ���
IMAGE TitleLogo;	//�^�C�g�����S
IMAGE TitleEnter;	//�G���^�[�L�[������
IMAGE EndClear;		//�N���A���S

//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//���ʉ�
AUDIO PlayerSE;
AUDIO GoalSE;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;	//�t�F�[�h�C��

int fadeTimeMill = 2000;  //�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60; //�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;		//�����l
int fadeInCnt = fadeInCntInit;			//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = fadeTimeMax;			//�t�F�[�h�C���̃J�E���^MAX

//PushEnter�̓_��
int PushEnterCnt = 0;			//�J�E���^
const int PushEnterCntMax = 60;	//�J�E���^MAX�l
BOOL PushEnterBrink = FALSE;	//�_�ł��Ă��邩�H

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(�����j
VOID TitleDraw(VOID);	//�^�C�g�����(�`��j

VOID Play(VOID);	//�v���C���
VOID PlayProc(VOID);//�v���C���(�����j
VOID PlayDraw(VOID);//�v���C���(�`��)

VOID End(VOID);		//�G���h���
VOID EndProc(VOID); //�G���h���(����)
VOID EndDraw(VOID); //�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene); //�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara); //�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);       //�����蔻��

BOOL OnCollRECT(RECT a, RECT b);		//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID); //�Q�[���̃f�[�^�̓ǂݍ���

VOID GameInit(VOID); //�Q�[���̃f�[�^�̏�����

BOOL LoadImageMem(IMAGE* image, const char* path);	//�Q�[���摜�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//�Q�[�����y�̓ǂݍ���

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);              //Log,txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);                            //�E�B���h�E���[�h�̐ݒ�
	SetMainWindowText(GAME_TITLE);                     //�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR); //�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);            //�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);                 //�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);                     //�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);               //�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);                            //�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);                            //�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{

		return -1;	// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();
		return -1;
	}
	
	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) {break;}//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) {break;}

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break;}

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}


		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();		//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();		//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene; //���̃V�[���̕ۑ�
				GameScene = GAME_SCENE_CHANGE; //��ʐ؂�ւ��V�[���ɕς���
			}
		}

		
		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);		//�摜���������ォ��폜

	DeleteGraph(player.img.handle);			//�摜���������ォ��폜
	DeleteGraph(Goal.img.handle);			//�摜���������ォ��폜

	DeleteGraph(TitleLogo.handle);			//�摜���������ォ��폜
	DeleteGraph(TitleEnter.handle);			//�摜���������ォ��폜
	DeleteGraph(EndClear.handle);			//�摜���������ォ��폜

	DeleteSoundMem(TitleBGM.handle);	//���y���������ォ��폜
	DeleteSoundMem(PlayBGM.handle);	    //���y���������ォ��폜
	DeleteSoundMem(EndBGM.handle);	    //���y���������ォ��폜

	DeleteSoundMem(PlayerSE.handle);	    //���y���������ォ��폜
	DeleteSoundMem(GoalSE.handle);	    //���y���������ォ��폜

	

	DxLib_End();						// �c�w���C�u�����g�p�̏I������

	return 0;							// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE/�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\Jet - 33479.mp4"); //�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path); //�摜�̓ǂݍ���



	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			playMovie.path,
			"�摜�ǂݍ��݃G���[!",
			MB_OK
		);

		return FALSE; //�ǂݍ��ݎ��s
	}

	// �摜�̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//����̃{�����[��
	playMovie.Volume = 255;

	//�摜��ǂݍ���
	if (!LoadImageMem(&player.img, ".\\images\\player.png")) { return FALSE; }
	if (!LoadImageMem(&Goal.img, ".\\images\\Goal.png")) { return FALSE; }

	//���S�̓ǂݍ���
	if (!LoadImageMem(&TitleLogo, ".\\images\\�^�C�g�����S.png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter, ".\\images\\PSUH ENTER.png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\images\\Game Clear.png")) { return FALSE; }

	//���y��ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\nagagutudeodekake.mp3",255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\natsuyasuminotanken.mp3",255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\genjitsunosukima.mp3",255, DX_PLAYTYPE_LOOP)) { return FALSE; }

	if (!LoadAudio(&PlayerSE, ".\\Audio\\����A�{�^������1.mp3",255, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!LoadAudio(&GoalSE, ".\\Audio\\�����Ɣ���.mp3",255, DX_PLAYTYPE_BACK)) { return FALSE; }

	
	return TRUE; //���ׂēǂݍ��߂��I

}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	strcpyDx(image->path, path); //�p�X�̃R�s�[
	image->handle = LoadGraph(image->path); //�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			image->path,
			"�摜�ǂݍ��݃G���[!",
			MB_OK
		);

		return FALSE; //�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path); //�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path); //���y�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"�摜�ǂݍ��݃G���[!",
			MB_OK
		);

		return FALSE; //�ǂݍ��ݎ��s
	}

	audio->playType = playType;	
	audio->Volume = volume;
}

/// <summary>
/// �Q�[���̃f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[�̏�����
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;  //������
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2; //������
	player.speed = 500;	   //�X�s�[�h
	player.img.IsDraw = TRUE;  //�`��ł���I

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�v���C���[�̏�����
	Goal.img.x = 0;
	Goal.img.y = 0;
	Goal.speed = 500;	   //�X�s�[�h
	Goal.img.IsDraw = TRUE;  //�`��ł���I

	//�����蔻����X�V����
	CollUpdate(&Goal);

	//�^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2; //��������
	TitleLogo.y = 100;

	//PushEnter�̈ʒu�����߂�
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2; //��������
	TitleEnter.y = GAME_HEIGHT  - TitleEnter.height - 100; 

	//PushEnter�̓_��
	int PushEnterCnt = 0;	
	//int PushEnterCntMax = 60;	
	BOOL PushEnterBrink = FALSE;

	//�N���A���S�̈ʒu�����߂�
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;	//��������
	EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2; //��������


}

	/// <summary>
	/// �V�[����؂�ւ���֐�
	/// </summary>
	/// <param name="scene">�V�[��</param>
	VOID ChangeScene(GAME_SCENE scene)
	{
		GameScene = scene; //�V�[����؂�ւ�
		IsFadeIn = FALSE;  //�t�F�[�h�C�����Ȃ�
		IsFadeOut = TRUE;  //�t�F�[�h�A�E�g����
}

	/// <summary>
	/// �^�C�g�����
	/// </summary>
	VOID Title(VOID)
	{
		TitleProc();	//����
		TitleDraw();	//�`��

		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̏���
	/// </summary>
	VOID TitleProc(VOID)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(TitleBGM.handle);

			//�V�[���؂�ւ�
			//�v���C��ʂɐ؂�ւ�

			//�Q�[���̏�����
			GameInit();

			ChangeScene(GAME_SCENE_PLAY);

			return;
		}

		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(TitleBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
		}

		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̕`��
	/// </summary>
	VOID TitleDraw(VOID)
	{

		//�^�C�g�����S�̕`��
		DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);
		
		//MAX�l�܂ő҂�
		if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
		else
		{
			if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
			else if (PushEnterBrink == FALSE)PushEnterBrink == TRUE;

			PushEnterCnt = 0; //�J�E���^�̏�����
		}

		//PushEnter��_��
		if (PushEnterBrink == TRUE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMax) * 255); 
			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		if (PushEnterBrink == FALSE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMax - PushEnterCnt) / PushEnterCntMax) * 255);
			//PushEnter�̕`��
			DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
		/// �v���C���
		/// </summary>
	VOID Play(VOID)
	{
		PlayProc();	//����
		PlayDraw();	//�`��

		return;
	}

	/// <summary>
	/// �v���C��ʂ̏���
	/// </summary>
	VOID PlayProc(VOID)
	{
		//if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		//{
		//	//�V�[���؂�ւ�
		//	//�G���h��ʂɐ؂�ւ�
		//	ChangeScene(GAME_SCENE_END);
		//}
		

		// BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(PlayBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
		}

		//�v���C���[�̑���
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			player.img.y -= player.speed * fps.DeltaTime;

			//�������̌��ʉ���ǉ�
			if (CheckSoundMem(PlayerSE.handle) == 0)
			{
				PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
			}

		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			player.img.y += player.speed * fps.DeltaTime;

			//�������̌��ʉ���ǉ�
			if (CheckSoundMem(PlayerSE.handle) == 0)
			{
				PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
			}
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			player.img.x -= player.speed * fps.DeltaTime;

			//�������̌��ʉ���ǉ�
			if (CheckSoundMem(PlayerSE.handle) == 0)
			{
				PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
			}

		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			player.img.x += player.speed * fps.DeltaTime;

			//�������̌��ʉ���ǉ�
			if (CheckSoundMem(PlayerSE.handle) == 0)
			{
				PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
			}

		}

		//�����蔻����X�V����
		CollUpdatePlayer(&player);

		//�����蔻����X�V����
		CollUpdate(&Goal);

		//�v���C���[���S�[���ɓ�����������
		if (OnCollRECT(player.coll, Goal.coll) == TRUE)
		{
			//�S�[���ɓ����������Ɍ��ʉ���ǉ�
			if (CheckSoundMem(GoalSE.handle) == 0)
			{
				PlaySoundMem(GoalSE.handle, GoalSE.playType);
			}
			
			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);
			
			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

			//�����������I��
			return;
		}

		

		return;
	}

	/// <summary>
	/// �v���C��ʂ̕`��
	/// </summary>
	VOID PlayDraw(VOID)
	{
		//�w�i�����`��

		//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
		if (GetMovieStateToGraph(playMovie.handle) == 0)
		{
			//�Đ�
			SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
			PlayMovieToGraph(playMovie.handle);		//������Đ�
		}
		//�����`��(�摜���������΂��j
		DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);


		//�v���C���[�̕`��
		if (player.img.IsDraw == TRUE)
		{
			//�摜�̕`��
			DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

			//�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			if (GAME_DEBUG == TRUE)
			{
				//�l�p��`��
				DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
					GetColor(255, 0, 0), FALSE);
			}

		}

		//�S�[���̕`��
		if (Goal.img.IsDraw == TRUE)
		{
			//�摜�̕`��
			DrawGraph(Goal.img.x, Goal.img.y, Goal.img.handle, TRUE);

			//�f�o�b�O�̂Ƃ��́A�����蔻��̗̈��`��
			if (GAME_DEBUG == TRUE)
			{
				//�l�p��`��
				DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
					GetColor(255, 0, 0), FALSE);
			}
		}


		DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
		return;
	}

	/// <summary>
	/// �G���h���
	/// </summary>
	VOID End(VOID)
	{
		EndProc();	//����
		EndDraw();	//�`��

		return;
	}

	/// <summary>
	/// �G���h��ʂ̏���
	/// </summary>
	VOID EndProc(VOID)
	{
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{
			//BGM���~�߂�
			StopSoundMem(EndBGM.handle);
			
			//�V�[���؂�ւ�
			//�^�C�g����ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_TITLE);
		}
		
		// BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(EndBGM.handle) == 0)
		{
			//BGM�𗬂�
			PlaySoundMem(EndBGM.handle, EndBGM.playType);
		}

		return;
	}

	/// <summary>
	/// �G���h��ʂ̕`��
	/// </summary>
	VOID EndDraw(VOID)
	{

		//EndClear�̕`��
		DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

		DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));		
		return;
	}

	/// <summary>
	/// �؂�ւ����
	/// </summary>
	VOID Change(VOID)
	{
		ChangeProc();	//����
		ChangeDraw();	//�`��

		return;
	}

	/// <summary>
	/// �؂�ւ���ʂ̏���
	/// </summary>
	VOID ChangeProc(VOID)
	{
		
		//�t�F�[�h�C��
		if (IsFadeIn == TRUE)
		{
			if (fadeInCnt > fadeInCntMax)
			{
				fadeInCnt--;	//�J�E���^�����炷
			}
			else
			{
				fadeInCnt = fadeInCntInit;	//�J�E���^������
				IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
			}
		}

		//�t�F�[�h�A�E�g
		if (IsFadeOut == TRUE)
		{
			if (fadeOutCnt < fadeOutCntMax)
			{
				fadeOutCnt++;	//�J�E���^�𑝂₷
			}
			else
			{
				fadeOutCnt = fadeOutCntInit;	//�J�E���^������
				IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
			}
		}

		//�؂�ւ������I��
		if (IsFadeIn == FALSE && IsFadeOut == FALSE)
		{
			//�t�F�[�h�C���A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
			GameScene = NextGameScene; //���̃Q�[���V�[���ɐ؂�ւ�
			OldGameScene = GameScene;  //�ȑO�̃Q�[���V�[�����X�V
		}
		return;

		
	}

	/// <summary>
	/// �؂�ւ���ʂ̕`��
	/// </summary>
	VOID ChangeDraw(VOID)
	{
		//�ȑO�̃V�[����`��
		switch (OldGameScene)
		{
		case GAME_SCENE_TITLE:
			TitleDraw();	//�^�C�g����ʂ̕`��
			break;
		case GAME_SCENE_PLAY:
			PlayDraw();		//�v���C��ʂ̕`��
			break;
		case GAME_SCENE_END:
			EndDraw();		//�G���h��ʂ̕`��
			break;
		default:
			break;
		}

		//�t�F�[�h�C��
		if (IsFadeIn == TRUE)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);  //0�`255�܂�
		}

		//�t�F�[�h�A�E�g
		if (IsFadeOut == TRUE)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);  //0�`255�܂�
		}

		//�l�p��`��
		DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);
		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
		DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
		return;
	}


	/// <summary>
	/// �����蔻��̗̈�X�V
	/// </summary>
	/// <param name="chara">�����蔻��̗̈�</param>
	VOID CollUpdatePlayer(CHARACTOR* chara) 
	{
		chara->coll.left = chara->img.x + 120;
		chara->coll.top = chara->img.y;
		chara->coll.right = chara->img.x + chara->img.width;
		chara->coll.bottom = chara->img.y + chara->img.height;

		return;
	}

	/// <summary>
	/// �����蔻��̗̈�X�V
	/// </summary>
	/// <param name="chara">�����蔻��̗̈�</param>
	VOID CollUpdate(CHARACTOR* chara)
	{
		chara->coll.left = chara->img.x;
		chara->coll.top = chara->img.y;
		chara->coll.right = chara->img.x + chara->img.width;
		chara->coll.bottom = chara->img.y + chara->img.height;

		return;
	}

	/// <summary>
	/// ��`�Ƌ�`�̓����蔻��
	/// </summary>
	/// <param name="">��`A</param>
	/// <param name="">��`B</param>
	/// <returns>����������TRUE/������Ȃ��Ȃ�FALSE</returns>
	BOOL OnCollRECT(RECT a,RECT b)
	{
		if (a.left < b.right &&		//��`A�̍���X�̍��W < ��`B�̉E��Y�̍��W�@����
			a.right > b.left &&		//��`A�̉E��X�̍��W > ��`B�̍���Y�̍��W�@����
			a.top < b.bottom &&		//��`A�̏��Y�̍��W < ��`B�̉���Y�̍��W�@����
			a.bottom > b.top		//��`A�̉���Y�̍��W < ��`B�̏��Y�̍��W
			)
		{
			//�������Ă���Ƃ�
			return TRUE;
		}
		else
		{
			//�������Ă��Ȃ��Ƃ�
			return FALSE;
		}
	}