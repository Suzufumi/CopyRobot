/*!
@file MyUtil.h
@brief BaseCross�ɂ͂Ȃ����[�e�B���e�B�֐�
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	namespace MyUtil
	{
		/*!
		 *@brief wstring����float�֕ϊ�����
		 *@param[in] wstr wstring
		 *@return �ϊ����ꂽfloat
		 *@author Kyohei Okawa
		*/
		float wstrToFloat(wstring wstr);

		/*!
		 *@brief wstring����int�֕ϊ�����
		 *@param[in] wstr wstring
		 *@return �ϊ����ꂽint
		 *@author Kyohei Okawa
		*/
		int wstrToInt(wstring wstr);

		/*!
		 *@brief wstring����bool�֕ϊ�����
		 *@param[in] wstr wstring
		 *@return �ϊ����ꂽbool
		 *@details 0�̎���false�ɕϊ����ꂻ��ȊO��true�ɕϊ������
		 *@author Kyohei Okawa
		*/
		bool wstrToBool(wstring wstr);

		/*!
		 *@brief �ŏ��l�������l���ŏ��l�ɁA�ő�l�𒴂���l���ő�l�ɂ���
		 *@param[in] minVal �ŏ��l
		 *@param[in] maxVal �ő�l
		 *@param[in] value �ϊ�����l
		 *@return �ϊ���̒l
		 *@author Kyohei Okawa
		*/
		float clamp(float minVal, float maxVal, float value);

		/*!
		 *@brief �����𐔂���
		 *@param[in] number �Ώۂ̐���
		 *@return number�̌���
		 *@author Kyohei Okawa
		*/
		int getDigit(int number);

		/*!
		 *@brief �������ꌅ���킯vector�Ɏ��[����
		 *@param[in] number ����
		 *@param[in] digit number�̌���
		 *@return �ꌅ���̐����������Ă���vector�z��
		 *@details 0�Ԗڂɂ͂P�̈ʂ̐����A1�Ԗڂɂ�10�̈ʂ̐���...
		 *@author Kyohei Okawa
		*/
		std::vector<int> extractTotalDigitsSpotCount(int number, int digit);

		///Vec3��y������rotDeg������]������
		Vec3 rotateXZVec3(const Vec3 v, const float rotDeg);

		///unity��Vector3.ToString()�ŏo�͂��ꂽ������basecross��Vec3�ɕϊ�����
		Vec3 unityVec3StrToBCVec3(wstring str);

		///unity��Quaternion.ToString()�ŏo�͂��ꂽ������basecross��Quat�ɕϊ�����
		Quat unityQuatStrToBCQuat(wstring str);

		///�r���{�[�h���邽�߂̃N�I�[�^�j�I�����쐬����
		Quat createBillboard(Vec3 line);
	}

/*@}*/
}