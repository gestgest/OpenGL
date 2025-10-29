//////////////////////////////////////////
//		jieunlee@hansung.ac.kr			//
//		2020. 10. 12					//
//////////////////////////////////////////

#ifndef HUMAN_H
#define HUMAN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//팰비스 : 골반
// Bones in a Human skeleton
enum Human_Bone {		// 20 bones
    pelvis, spine, 
	neck, head, 
	clavicleL, upperarmL, forearmL, handL, 
	clavicleR, upperarmR, forearmR, handR,
	thighL, calfL, footL, toeL, 
	thighR, calfR, footR, toeR
};

//todo : 지금은 포즈 2개지만 더 만들어야 함
// several poses
enum Human_Pose {
	base, armLeftUp,idle, walking0, walking1,walking2, walking3,
};

// Default values
const int BONENUM = 20; // + 1
const int POSENUM = 7;



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Human
{
public:
    // Skeleton Attributes
   glm::quat	BoneRotate[BONENUM];

    // Constructor 
    Human()
    {
		// bone length
		BoneLength[pelvis] = 1.0f;		BoneLength[spine] = 3.0f;
		BoneLength[neck] = 1.0f;		BoneLength[head] = 1.0f;
		BoneLength[clavicleL] = 1.0f;	BoneLength[upperarmL] = 2.0f;	BoneLength[forearmL] = 1.5f;	BoneLength[handL] = 1.0f;
		BoneLength[clavicleR] = 1.0f;	BoneLength[upperarmR] = 2.0f;	BoneLength[forearmR] = 1.5f;	BoneLength[handR] = 1.0f;
		BoneLength[thighL] = 2.5f;		BoneLength[calfL] = 2.0f;		BoneLength[footL] = 1.0f;		BoneLength[toeL] = 0.5f;
		BoneLength[thighR] = 2.5f;		BoneLength[calfR] = 2.0f;		BoneLength[footR] = 1.0f;		BoneLength[toeR] = 0.5f;
		
		//todo : 여기를...?
		// setup poses
		SetupPoses();

		// bone rotation - base pose
		SetPose(base);
	}

	void drawObject(Shader &shader, unsigned int & cubeVAO, glm::mat4& bone, int index, glm::vec3 color, glm::vec3 addPos, glm::vec3 rotatePos, float thickSize = 1.0f)
	{
		bone = glm::translate(bone, addPos);

		bone = glm::translate(bone, rotatePos);
		bone = bone * glm::mat4_cast(BoneRotate[index]); //회전을 행렬로 변환 => 회전 원점은 왼쪽 아래
		bone = glm::translate(bone, -rotatePos);

		bone = glm::scale(bone, glm::vec3(thickSize, BoneLength[index], thickSize));
		shader.setMat4("model", bone);
		shader.setVec3("objectColor", color);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		bone = glm::scale(bone, glm::vec3(1.0f / thickSize, 1.0f / BoneLength[index], 1.0f / thickSize));
	}

	void SetPose(Human_Pose pose)
	{
		for (int i = pelvis; i <= toeR; i++)
		{
			BoneRotate[i] = Pose[pose][i];
			//std::cout << "x : " << BoneRotate[i].x << "\ny : " << BoneRotate[i].y << "\nz : " << BoneRotate[i].z << "\nw : " << BoneRotate[i].w << '\n';
		}
	}

	void SetBoneRotation(Human_Bone bone, glm::quat q)
	{
		BoneRotate[bone] = q;
	}

	//쿼터니언 믹스 => 포즈와 포즈를 합쳐서 모션
	void MixPose(Human_Pose p, Human_Pose q, float t)
	{
		//std::cout << "test";

		for (int i = pelvis; i <= toeR; i++)
		{
			BoneRotate[i] = glm::mix(Pose[p][i], Pose[q][i], t);
		}
	}

	// 그리기
	void DrawHuman(Shader shader, unsigned int cubeVAO, glm::mat4 model)
	{
		glm::mat4 bone = model;
		glm::mat4 mspine; //상체라인 => 나중에 기억해서 다시 복귀해야함
		glm::mat4 mpelvis = model; //골반 => 나중에 기억해서 다시 복귀해야함
		glm::vec3 defaultRotatePos = glm::vec3(0, 0, 0);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// draw pelvis
		bone = mpelvis;
		drawObject(shader, cubeVAO, bone, pelvis, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), defaultRotatePos);


		// draw spine
		drawObject(shader, cubeVAO, bone, spine, glm::vec3(1.0f, 0.5f, 0.3f), glm::vec3(0, BoneLength[pelvis], 0), defaultRotatePos);
		mspine = bone;

		//draw neck
		drawObject(shader, cubeVAO, bone, neck, glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(0, BoneLength[spine], 0), defaultRotatePos, 0.5f);

		// draw head
		drawObject(shader, cubeVAO, bone, head, glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0, BoneLength[neck], 0), defaultRotatePos);

		// draw clavicleL
		bone = mspine;
		drawObject(shader, cubeVAO, bone, clavicleL, glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.5f, BoneLength[spine], 0), defaultRotatePos, 0.5f);

		// draw upperarmL
		drawObject(shader, cubeVAO, bone, upperarmL, glm::vec3(0.3f, 0.0f, 0.7f), glm::vec3(0, BoneLength[clavicleL], 0), defaultRotatePos, 0.5f);

		// draw forearmL
		drawObject(shader, cubeVAO, bone, forearmL, glm::vec3(0.7f, 0.0f, 0.5f), glm::vec3(0, BoneLength[upperarmL], 0), defaultRotatePos, 0.5f);


		// draw handL
		drawObject(shader, cubeVAO, bone, handL, glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0, BoneLength[forearmL], 0), defaultRotatePos, 0.5f);


		// draw clavicleR
		bone = mspine;
		drawObject(shader, cubeVAO, bone, clavicleR, glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(-0.5f, BoneLength[spine], 0), defaultRotatePos, 0.5f);

		// draw upperarmR
		drawObject(shader, cubeVAO, bone, upperarmR, glm::vec3(0.3f, 0.0f, 0.7f), glm::vec3(0, BoneLength[clavicleR], 0), defaultRotatePos, 0.5f);

		// draw forearmR
		drawObject(shader, cubeVAO, bone, forearmR, glm::vec3(0.7f, 0.0f, 0.5f), glm::vec3(0, BoneLength[upperarmR], 0), defaultRotatePos, 0.5f);

		// draw handR
		drawObject(shader, cubeVAO, bone, handR, glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0, BoneLength[forearmR], 0), defaultRotatePos, 0.5f);

		// 하체
		// 오른쪽 하체
		// draw thighL
		bone = mpelvis;
		drawObject(shader, cubeVAO, bone, thighL, glm::vec3(0.5882353f, 0.29411765f, 0.0f),
			glm::vec3(0.75f, BoneLength[pelvis] -BoneLength[thighL], 0),
			glm::vec3(0, - BoneLength[pelvis] + BoneLength[thighL], 0),
			0.5f
		);

		// draw calfL
		drawObject(shader, cubeVAO, bone, calfL, glm::vec3(0.3882353f, 0.19411765f, 0.5f), glm::vec3(0.0f, -BoneLength[calfL], 0.0f), defaultRotatePos, 0.5f);

		// draw footL
		drawObject(shader, cubeVAO, bone, footL, glm::vec3(0.2882353f, 0.09411765f, 0.5f), glm::vec3(0.0f, -BoneLength[footL], 0.0f), defaultRotatePos, 0.5f);

		// draw toeL
		drawObject(shader, cubeVAO, bone, toeL, glm::vec3(0.7f, 0.7f, 0.5f), glm::vec3(0.0f, -BoneLength[toeL], 0.0f), defaultRotatePos, 0.5f);
		//std::cout << "x : " << BoneRotate[toeL].x << "'\ny : " << BoneRotate[toeL].y << "'\nz : " << BoneRotate[toeL].z << "'\nw : " << BoneRotate[toeL].w << '\n';

		//draw thighR
		bone = mpelvis;
		drawObject(shader, cubeVAO, bone, thighR, 
			glm::vec3(0.5882353f, 0.29411765f, 0.0f),
			glm::vec3(-0.75f, BoneLength[pelvis] - BoneLength[thighR], 0),
			glm::vec3(0, -BoneLength[pelvis] + BoneLength[thighR], 0),
			0.5f
		);

		// draw calfR
		drawObject(shader, cubeVAO, bone, calfR, glm::vec3(0.3882353f, 0.19411765f, 0.5f), glm::vec3(0.0f, -BoneLength[calfR], 0.0f), defaultRotatePos, 0.5f);

		// draw footR
		drawObject(shader, cubeVAO, bone, footR, glm::vec3(0.2882353f, 0.09411765f, 0.5f), glm::vec3(0.0f, -BoneLength[footR], 0.0f), defaultRotatePos, 0.5f);
		//std::cout << "x : " << BoneRotate[footR].x << "'\ny : " << BoneRotate[footR].y << "'\nz : " << BoneRotate[footR].z << "'\nw : " << BoneRotate[footR].w << '\n';

		// draw toeR
		drawObject(shader, cubeVAO, bone, toeR, glm::vec3(0.7f, 0.7f, 0.5f), glm::vec3(0.0f, -BoneLength[toeR], 0.0f), defaultRotatePos, 0.5f);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

private:
	float		BoneLength[BONENUM];
	glm::quat	Pose[POSENUM][BONENUM]; //todo : 포즈를 더 만들어라

	void SetupPoses()
	{
		for (int p = 0; p < POSENUM; p++)
		{
			for (int i = 0; i < BONENUM; i++)
			{
				Pose[p][i] = glm::angleAxis(glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
			}

			switch (p) {
				//90도
			case base: //default pose
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f)); //시작점 -90
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f)); //시작점 90
				break;
			case armLeftUp: //왼쪽 팔 드는거
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f)); //고정
				Pose[p][upperarmL] = glm::angleAxis(glm::radians(50.f), glm::vec3(0.f, 0.f, 1.f));
				Pose[p][forearmL] = glm::angleAxis(glm::radians(50.f), glm::vec3(0.f, 0.f, 1.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f)); // 고정
				break;
			case idle:
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-180.f), glm::vec3(0.f, 0.f, 1.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
				break;
			case walking0: //걷는 거
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-180.f), glm::vec3(0.f, 0.f, 1.f))
					* glm::angleAxis(glm::radians(-45.0f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f)
					* glm::angleAxis(glm::radians(45.0f), glm::vec3(1.f, 0.f, 0.f)));

				Pose[p][thighL] = glm::angleAxis(glm::radians(45.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][calfL] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][footL] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][toeL] = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][thighR] = glm::angleAxis(glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][calfR] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[walking0][footR] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[walking0][toeR] = glm::angleAxis(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
				break;

			case walking1:
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-180.f), glm::vec3(0.f, 0.f, 1.f))
					* glm::angleAxis(glm::radians(0.0f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f)
					* glm::angleAxis(glm::radians(0.0f), glm::vec3(1.f, 0.f, 0.f)));

				Pose[p][thighL] = glm::angleAxis(glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][calfL] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][calfR] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][thighR] = glm::angleAxis(glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
				break;

			case walking2:
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-180.f), glm::vec3(0.f, 0.f, 1.f))
					* glm::angleAxis(glm::radians(45.0f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f)
					* glm::angleAxis(glm::radians(-45.0f), glm::vec3(1.f, 0.f, 0.f)));

				Pose[p][thighL] = glm::angleAxis(glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][thighR] = glm::angleAxis(glm::radians(45.f), glm::vec3(1.f, 0.f, 0.f));

				//Pose[p][calfL] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][calfR] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				break;

			case walking3:
				Pose[p][clavicleL] = glm::angleAxis(glm::radians(-180.f), glm::vec3(0.f, 0.f, 1.f))
					* glm::angleAxis(glm::radians(0.0f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][clavicleR] = glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f)
					* glm::angleAxis(glm::radians(0.0f), glm::vec3(1.f, 0.f, 0.f)));

				Pose[p][thighL] = glm::angleAxis(glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
				Pose[p][thighR] = glm::angleAxis(glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));

				//Pose[p][calfL] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				//Pose[p][calfR] = glm::angleAxis(glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
				break;
			default:
				break;
			};
		}
	}

};
#endif

//thighL, calfL, footL, toeL, 
//thighR, calfR, footR, toeR