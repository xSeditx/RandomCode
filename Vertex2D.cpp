//                                                                                                                                                                                                                                                                                                
//                                                                                                                                                                                                                                                                                                
//   Vector Library cut from my original to just include 2D vectors                                                                                                                                                                                                                                                                            
//                                                                                                                                                                                                                                                                                                
//                                                                                                                                                                                                                                                                                             
//                                                                                                                                                                                                                                                                                             
//                                                                                                                                                                                                                                                                                             
// TODO: Template the Overloads that allow addition of non Vec2D values to Vertices                                                                                                                                                                                                                                                                                      
//==================================================================================================================================                                                                                                                                                                                                                                                                                             


#include"Vertex2D.h"

Vector2D::Vector2D(){X = 0.0f;Y=0.0f;}
Vector2D::Vector2D(const float& x,const float& y){this->X=x;this->Y=y;}

//==================================================================================================================================
//__________________________________________________________________________________________________________________________________

Vector2D& Vector2D::add(const Vector2D& other){
    X += other.X; Y += other.Y;
    return *this;
}
Vector2D& Vector2D::subtract(const Vector2D& other){
    X -= other.X; Y -= other.Y;
    return *this;
}
Vector2D& Vector2D::divide(const Vector2D& other){
    X /= other.X; Y /= other.Y;
    return *this;
}
Vector2D& Vector2D::multiply(const Vector2D& other){
	X *= other.X; Y *= other.Y;
	return *this;
}

Vector2D operator +(Vector2D left, const Vector2D& right){return left.add(right);}
Vector2D operator -(Vector2D left, const Vector2D& right){return left.subtract(right);}
Vector2D operator /(Vector2D left, const Vector2D& right){return left.divide(right);}
Vector2D operator *(Vector2D left, const Vector2D& right){return left.multiply(right);}

Vector2D& Vector2D::operator +=(const Vector2D& other){return add(other);}
Vector2D& Vector2D::operator -=(const Vector2D& other){return subtract(other);}
Vector2D& Vector2D::operator *=(const Vector2D& other){return multiply(other);}
Vector2D& Vector2D::operator /=(const Vector2D& other){return divide(other);}

//==================================================================================================================================
//___________  My DOUBLE OVERLOADS to ADD A SINGLE VALUE TO BOTH ELEMENTS OF THE VECTOR ____________________________________________
//__________________________________________________________________________________________________________________________________
Vector2D& Vector2D::add(float other){
    X += other; Y += other;
    return *this;
}
Vector2D& Vector2D::subtract(float other){
    X -= other; Y -= other;
    return *this;
}
Vector2D& Vector2D::divide(float other){
    X /= other; Y /= other;
    return *this;
}
Vector2D& Vector2D::multiply(float other){
    X *= other; Y *= other;
    return *this;
}
Vector2D operator +(Vector2D left, float right){return left.add(right);}
Vector2D operator -(Vector2D left, float right){return left.subtract(right);}
Vector2D operator /(Vector2D left, float right){return left.divide(right);}
Vector2D operator *(Vector2D left, float right){return left.multiply(right);}



Vector2D& Vector2D::operator =(float right){return this->assign(right);}


Vector2D& Vector2D::operator +=(float other){return add(other);}
Vector2D& Vector2D::operator -=(float other){return subtract(other);}
Vector2D& Vector2D::operator *=(float other){return multiply(other);}
Vector2D& Vector2D::operator /=(float other){return divide(other);}
//__________________________________________________________________________________________________________________________________
//__________________________________________________________________________________________________________________________________


Vector2D& Vector2D::assign(float other){
    X = other; Y = other;
    return *this;
}


bool Vector2D::operator ==(const Vector2D& other){return X == other.X && Y == other.Y;}
bool Vector2D::operator !=(const Vector2D& other){return !(*this == other);}
//__________________________________________________________________________________________________________________________________

std::ostream& operator<<(std::ostream& stream,const Vector2D& vector){
    stream << "X:(" << vector.X <<")  Y:("<< vector.Y<<")";
    return stream;
}


float Vector2D::Dot(Vector2D A, Vector2D B)
{
    return A.X*B.X+A.Y*B.Y;
}

float Vector2D::Cross(Vector2D A, Vector2D B)
{
    return A.X*B.Y-A.Y*B.X;
}



inline float Squared(float x){
    return x*x;
}
inline float GetAngle(Vector2D A, Vector2D B)
{
    return atan2f(B.Y - A.Y,B.X - A.X ) * ( 180 /  3.14159) ;
}




inline Vector2D Get_Displacement(Vector2D A, Vector2D B){
    Vector2D ret;
    ret.Y = A.Y - B.Y;
    ret.X = A.X - B.X;
    return ret;
}






