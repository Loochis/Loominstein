// mathchis.h

#ifndef MATHCHIS_H
#define MATHCHIS_H

#include <vector>

namespace MATHCHIS
{
    // COLUMN-MAJOR MATRIX
    class matrix {
    public:
        int n, m;
        std::vector<float> elems;

        matrix(int n_d, int m_d);
        explicit matrix(int n_d, int m_d, std::vector<float> o_elems);

        matrix  operator+(matrix const &obj) const;
        matrix& operator+=(matrix const &obj);
        matrix  operator-(matrix const &obj) const;
        matrix& operator-=(matrix const &obj);

        matrix  operator*(matrix const &obj) const; // SCALE OP
        matrix& operator*=(matrix const &obj); // SCALE OP
    };

	class vector
	{
	public:
        std::vector<float> elems;

		explicit vector(std::vector<float> o_elems);
		vector(float x, float y, float z);
		explicit vector(float o);
        vector();

		vector  operator+(vector const &obj) const;
        vector& operator+=(vector const &obj);
		vector  operator-(vector const &obj) const;
        vector& operator-=(vector const &obj);
		vector  operator*(float const &obj) const; // SCALE OP
        vector& operator*=(float const &obj); // SCALE OP
		vector  operator/(float const &obj) const; // INV-SCALE OP
        vector& operator/=(float const &obj); // INV-SCALE OP

        vector  operator*(vector const &obj) const; // ELEM MULTIPLY OP
        vector& operator*=(vector const &obj); // ELEM MULTIPLY OP

        vector  operator*(matrix const &obj) const; // MATRIX OP
        vector& operator*=(matrix const &obj); // MATRIX OP

        int normalize();

        vector reflect(vector &norm);

        // COLOR OPS
        int col_tobytes();

        // STATIC OPS
		static float dot(vector const &v1, vector const &v2);  // DOT OP
		static vector cross(vector const &v1, vector const &v2); // CROSS OP

        static float mag2(vector const &vec);
        static float mag(vector const &vec);
        static vector normalized(vector const &vec);
		
	private:

	};
}

#endif
