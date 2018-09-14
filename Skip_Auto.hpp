#ifndef SKIP_AUTO_H_
#define SKIP_AUTO_H_

#include <cstdint>

//���ǃX�L�b�v�f�[�^��������
void SkipDataWrite(const std::int32_t& RouteNum) noexcept;

//���ǔ���
constexpr void SkipDataCheck(const std::int32_t& RouteNum) noexcept;

//���ǃX�L�b�v�f�[�^�̓Ǎ�
int SkipDataLoad() noexcept;

//SKIP_READ SAVE�֐�
int SkipDataSave() noexcept;

// ���ǃX�L�b�v/�X�L�b�v/�I�[�g�؂�ւ��֐�
void SkipAndAutoTask(const std::int32_t& Num, const int Flag) noexcept;

#endif // !SKIP_AUTO_H_

