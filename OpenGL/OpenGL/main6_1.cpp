#include <iostream>	
#include <math.h>

class Vec {
public:
	float v[3];
	Vec() {
		for (int i = 0; i < 3; i++)
			v[i] = 0;
	}
	Vec(float a, float b, float c) {
		v[0] = a;
		v[1] = b;
		v[2] = c;
	}

	void print() {
		for (int i = 0; i < 3; i++)
			std::cout << v[i] <<", ";
	}
	float length() {
		float len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return len;
	}
	float inner(Vec vec) {
		float sum = 0;
		for (int i = 0; i < 3; i++)
			sum += v[0] * vec.v[0];
		return sum;
	}

	Vec outer(Vec vec) {
		Vec result;
		result.v[0] = v[1] * vec.v[2] - v[2] * vec.v[1];
		result.v[1] = v[2] * vec.v[0] - v[0] * vec.v[2];
		result.v[2] = v[0] * vec.v[1] - v[1] * vec.v[0];

		return result;
	}
};

class Mat {
public:
	int s[3][3];
	//매개변수 없으면 다 0
	Mat(){
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				s[i][j] = 0;
	}
	//단위 행렬
	void unitMat()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				if (i == j)
					s[i][j] = 1;
				else
					s[i][j] = 0;

			}
	}
	Mat transposMat()
	{
		Mat result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
					result.s[j][i] = s[i][j];
		return result;
	}
	
	void setMat(int value, int x, int y)
	{
		if (3 < x || 3 < y || x < 0 || y < 0)
			std::cout << "범위 오버";
		else
			s[y][x] = value;
	}

	void printMat()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				std::cout << s[i][j];
			std::cout << '\n';
		}
	}

	Mat *operator+(Mat b)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				s[i][j] += b.s[i][j];
		return this;
	}
	Mat* operator-(Mat b)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				s[i][j] -= b.s[i][j];
		return this;
	}

	Mat operator* (Mat b) {
		Mat result;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					result.s[i][j] += s[i][k] * b.s[k][j]; //중요하다

		return result;

	}
};

int main()
{
	Mat mat;
	Mat mat_sub;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			mat.setMat(3 * i + j + 1, j, i);
		}
	
	mat_sub.unitMat();
	mat = mat.transposMat();

	mat = mat_sub * mat ;

	//Vec vec1(2.0f, 1.0f, 0);
	//Vec vec2(0, 1.0f, 1.0f);
	//vec1.outer(vec2).print();
	//mat.printMat();
}