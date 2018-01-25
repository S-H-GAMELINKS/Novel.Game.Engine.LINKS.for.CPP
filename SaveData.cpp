// SaveData Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include <string>
#include <thread>
#include <chrono>

extern int SP, CP;	// �Q�Ƃ��镶����ԍ��ƕ����񒆂̕����|�C���^

//�e��f�ރn���h��
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;

//�I���t���O
extern int EndFlag;

namespace {

	//�Z�[�u�f�[�^�p�X�N���[���V���b�g�i�[�ϐ�
	std::int32_t SaveSnap[save_max_num];

	//�ʏ�Z�[�u�f�[�^
	struct alignas(4) SaveData_t {
		std::int32_t ENDFLAG;    //ENDFLAG
		std::int32_t SP;			//�s��
		std::int32_t CP;			//�����ʒu
		std::int32_t CHAR;		//�����G���
		std::int32_t BG;			//�w�i�摜���
		std::int32_t BGM;		//BGM���
	};

	//�Z�[�u�f�[�^�p�X�N���[���V���b�g�Ǎ��֐�
	void SaveDataSnapLoad() {
		std::string FilePath = "DATA/SAVE/SAVESNAP";
		std::string FileFormat = ".png";
		std::string FileName = "";

		for (std::int32_t i = 0; i < save_max_num; i++) {

			FileName = (FilePath + std::to_string(i) + FileFormat);

			SaveSnap[i] = DxLib::LoadGraph(FileName.c_str());
		}
	}

	//�Z�[�u/���[�h���j���[�`��
	void SaveLoadMenuDraw(std::int32_t& cursor_y) {

		//�X�N���[���V���b�g�`��
		for (std::int32_t i = 0; i < save_max_num; i++)
			DxLib::DrawRotaGraph(save_snap_draw_pos_x, save_base_pos_y * (i + 1), 0.2f, 0, SaveSnap[i - 1], TRUE);

		//�J�[�\���̕`��
		DxLib::DrawString(save_base_pos_x, cursor_y, "��", 255);

		//�Z�[�u�f�[�^���`��
		DxLib::DrawString(save_name_pos_x, save_base_pos_y, "�Z�[�u�f�[�^1", 255);
		DxLib::DrawString(save_name_pos_x, save_base_pos_y * 2, "�Z�[�u�f�[�^2", 255);
		DxLib::DrawString(save_name_pos_x, save_base_pos_y * 3, "�Z�[�u�f�[�^3", 255);

		DxLib::DrawString(save_name_pos_x - cursor_move, save_base_pos_y * 4, "�߂�", 255);
	}

	//�Z�[�u/���[�h���j���[�L�[����
	void SaveLoadMenuKeyMove(std::int32_t& cursor_y) {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (save_buttom_y == cursor_y) ? save_base_pos_y : cursor_y + save_move_unit;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (save_base_pos_y == cursor_y) ? save_buttom_y : cursor_y - save_move_unit;

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	//�Z�[�u�f�[�^���Z�[�u
	int SaveDataSave(const char* SaveDataPath) {

		SaveData_t SaveData = { EndFlag, SP, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

		FILE *fp;

#ifdef LINKS_HAS_FOPEN_S
		const errno_t er = fopen_s(&fp, SaveDataPath, "wb");
		if (0 != er || nullptr == fp) {
			return 0;
		}
#else
		fopen_s(&fp, SaveDataPath, "wb");//�o�C�i���t�@�C�����J��
		if (nullptr == fp) {//�G���[���N������nullptr��Ԃ�
			return 0;
		}
#endif

		fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t�\���̂̒��g���o��
		fclose(fp);

	}

	//�Z�[�u�f�[�^�����[�h
	int SaveDataLoad(const char* SaveDataPath) {

		SaveData_t SaveData;

		FILE *fp;

#ifdef LINKS_HAS_FOPEN_S
		const errno_t er = fopen_s(&fp, SaveDataPath, "rb");
		if (0 != er || nullptr == fp) {
			MessageBoxOk(ErrorMessage);
			return 0;
		}
#else
		fopen_s(&fp, SaveDataPath, "rb");
		if (fp == nullptr) {
			//MessageBoxOk(ErrorMessage);
			return 0;
		}
#endif
		fread(&SaveData, sizeof(SaveData), 1, fp);
		fclose(fp);
		EndFlag = SaveData.ENDFLAG;
		SP = SaveData.SP;
		CP = SaveData.CP;
		CharacterHandle = SaveData.CHAR;
		BackGroundHandle = SaveData.BG;
		BackGroundMusicHandle = SaveData.BGM;
	}

	//�Z�[�u�f�[�^���f���[�g
	void SaveDataDelete(const char* SaveDataPath) {
		std::remove(SaveDataPath);
	}

	//�Z�[�u/���[�h/�f���[�g�؂�ւ��֐�
	void SaveDataTask(std::int32_t Num, const char* SaveDataPath) {

		//�Z�[�u
		if (Num == 1)
			SaveDataSave(SaveDataPath);

		//���[�h
		if (Num == 2)
			SaveDataLoad(SaveDataPath);

		//�f���[�g
		if (Num == 3)
			SaveDataDelete(SaveDataPath);
	}
}

//�Z�[�u/���[�h���j���[�I������
void SaveLoadMenuSelect(std::int32_t& cursor_y) {

	if (cursor_y == save_base_pos_y && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		SaveDataTask(1, "DATA/SAVE/SAVEDATA1.bat");
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 2 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 3 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 4 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}
}