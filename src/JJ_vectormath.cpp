//Vectors
struct vec2 { f32 x, y;};

struct vec3 { f32 x, y, z;};

struct vec4 { f32 x, y, z, w; };

bool32 operator!=(vec3 A, vec3 B)
{
    bool32 result = 0;
    result += (A.x != B.x);
    result += (A.y != B.y);
    result += (A.z != B.z);
    if(result > 0){return 1;}
    return 0;
}

bool32 operator==(vec3 A, vec3 B)
{
    bool32 result = 0;
    result += (A.x == B.x);
    result += (A.y == B.y);
    result += (A.z == B.z);
    if(result == 3){return 1;}
    return 0;
}

bool32 operator ==(vec2 A, vec2 B)
{
    bool32 result = 0;
    result += (A.x == B.x);
    result += (A.y == B.y);
    if(result == 2){return 1;}
    return 0;
}

vec2 operator-(vec2 A)
{
	vec2 result;
	result.x = -A.x;
	result.y = -A.y;
	return(result);
}
vec3 operator-(vec3 A)
{
	vec3 result;
	result.x = -A.x;
	result.y = -A.y;
	result.z = -A.z;
	return(result);
}
vec4 operator-(vec4 A)
{
	vec4 result;
	result.x = -A.x;
	result.y = -A.y;
	result.z = -A.z;
	return(result);
}

vec2 operator+(vec2 A, vec2 B)
{
	vec2 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	return(result);
}
vec3 operator+(vec3 A, vec3 B)
{
	vec3 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	result.z = A.z + B.z;
	return(result);
}
vec4 operator+(vec4 A, vec4 B)
{
	vec4 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	result.z = A.z + B.z;
	result.w = A.w + B.w;
	return(result);
}

void operator+=(vec2 &A, vec2 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
}
void operator+=(vec3 &A, vec3 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
    A.z =  A.z + B.z;
}
void operator+=(vec4 &A, vec4 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
    A.z =  A.z + B.z;
    A.w =  A.w + B.w;
}

vec2 operator-(vec2 A, vec2 B)
{
	vec2 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	return(result);
}
vec3 operator-(vec3 A, vec3 B)
{
	vec3 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	result.z = A.z - B.z;
	return(result);
}
vec4 operator-(vec4 A, vec4 B)
{
	vec4 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	result.z = A.z - B.z;
	result.w = A.w - B.w;
	return(result);
}

vec2 operator*(f32 x, vec2 A)
{
	vec2 result;
	result.x = x * A.x;
	result.y = x * A.y;
	return(result);
}
vec3 operator*(f32 x, vec3 A)
{
	vec3 result;
	result.x = x * A.x;
	result.y = x * A.y;
	result.z = x * A.z;
	return(result);
}
vec4 operator*(f32 x, vec4 A)
{
	vec4 result;
	result.x = x * A.x;
	result.y = x * A.y;
	result.z = x * A.z;
	result.w = x * A.w;
	return(result);
}

f32 dot_vec2(vec2 A, vec2 B) { return (A.x * B.x) + (A.y * B.y); }
f32 dot_vec3(vec3 A, vec3 B) { return (A.x * B.x) + (A.y * B.y) + (A.z * B.z); }
f32 dot_vec4(vec4 A, vec4 B) { return (A.x * B.x) + (A.y * B.y) + (A.z * B.z) + (A.w * B.w); }

vec3 cross(vec3 A, vec3 B)
{
	vec3 result;
	result.x = (A.y*B.z - A.z*B.y);
	result.y = (A.z*B.x - A.x*B.z);
	result.z = (A.x*B.y - A.y*B.x);
	return(result);
}

//Matricies
struct mat2x2 { f32 d[2][2]; };
struct mat3x3 { f32 d[3][3]; };
struct mat3x4 { f32 d[4][3]; };
struct mat4x4 { f32 d[4][4]; };

mat3x3 rotate3x3X(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = 1; result.d[0][1] = 0; result.d[0][2] = 0;
    result.d[1][0] = 0; result.d[1][1] = cosf(theta); result.d[1][2] = -sinf(theta);
    result.d[2][0] = 0; result.d[2][1] = sinf(theta); result.d[2][2] = cosf(theta);
    return result;
}
mat3x3 rotate3x3Y(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = cosf(theta); result.d[0][1] = 0; result.d[0][2] = sinf(theta);
    result.d[1][0] = 0; result.d[1][1] = 1; result.d[1][2] = 0;
    result.d[2][0] = -sinf(theta); result.d[2][1] = 0; result.d[2][2] = cosf(theta);
    return result;
}
mat3x3 rotate3x3Z(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = cosf(theta); result.d[0][1] = -sinf(theta); result.d[0][2] = 0;
    result.d[1][0] = sinf(theta); result.d[1][1] = cosf(theta); result.d[1][2] = 0;
    result.d[2][0] = 0; result.d[2][1] = 0; result.d[2][2] = 1;
    return result;
}

mat3x3 operator*(mat3x3 A, mat3x3 B)
{
	mat3x3 result;
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]);
	return(result);
}

mat3x4 operator*(mat3x4 A, mat3x4 B)
{
	mat3x4 result;
	//Note on each of these rows. The mul by zero is implicit here. As i'd waste a mul and add zero otherwise if 4x4. So why not save some instructions!
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]);
	//Note on the final add here. Since i'm implying that the final value in the "w" slot is a 1. I just add the B and don't have an extra mul!
	result.d[3][0] = (A.d[3][0] * B.d[0][0]) + (A.d[3][1] * B.d[1][0]) + (A.d[3][2] * B.d[2][0]) + (B.d[3][0]); result.d[3][1] = (A.d[3][0] * B.d[0][1]) + (A.d[3][1] * B.d[1][1]) + (A.d[3][2] * B.d[2][1]) + (B.d[3][1]); result.d[3][2] = (A.d[3][0] * B.d[0][2]) + (A.d[3][1] * B.d[1][2]) + (A.d[3][2] * B.d[2][2]) + (B.d[3][2]);
	return(result);
}

mat4x4 operator*(mat4x4 A, mat4x4 B)
{
	mat4x4 result;
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]) + (A.d[0][3] * B.d[3][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]) + (A.d[0][3] * B.d[3][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]) + (A.d[0][3] * B.d[3][2]); result.d[0][3] = (A.d[0][0] * B.d[0][3]) + (A.d[0][1] * B.d[1][3]) + (A.d[0][2] * B.d[2][3]) + (A.d[0][3] * B.d[3][3]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]) + (A.d[1][3] * B.d[3][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]) + (A.d[1][3] * B.d[3][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]) + (A.d[1][3] * B.d[3][2]); result.d[1][3] = (A.d[1][0] * B.d[0][3]) + (A.d[1][1] * B.d[1][3]) + (A.d[1][2] * B.d[2][3]) + (A.d[1][3] * B.d[3][3]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]) + (A.d[2][3] * B.d[3][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]) + (A.d[2][3] * B.d[3][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]) + (A.d[2][3] * B.d[3][2]); result.d[2][3] = (A.d[2][0] * B.d[0][3]) + (A.d[2][1] * B.d[1][3]) + (A.d[2][2] * B.d[2][3]) + (A.d[2][3] * B.d[3][3]);
	result.d[3][0] = (A.d[3][0] * B.d[0][0]) + (A.d[3][1] * B.d[1][0]) + (A.d[3][2] * B.d[2][0]) + (A.d[3][3] * B.d[3][0]); result.d[3][1] = (A.d[3][0] * B.d[0][1]) + (A.d[3][1] * B.d[1][1]) + (A.d[3][2] * B.d[2][1]) + (A.d[3][3] * B.d[3][1]); result.d[3][2] = (A.d[3][0] * B.d[0][2]) + (A.d[3][1] * B.d[1][2]) + (A.d[3][2] * B.d[2][2]) + (A.d[3][3] * B.d[3][2]); result.d[3][3] = (A.d[3][0] * B.d[0][3]) + (A.d[3][1] * B.d[1][3]) + (A.d[3][2] * B.d[2][3]) + (A.d[3][3] * B.d[3][3]);
	return(result);
}

vec2 operator*(mat2x2 M, vec2 V)
{
    vec2 result;
	result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y);
	result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y);
	return(result);
}

vec3 operator*(mat3x3 M, vec3 V)
{
	vec3 result;
	result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z);
	result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z);
	result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z);
	return(result);
}

vec3 operator*(mat3x4 M, vec3 V) //NOTE: this ASSUMES that you are translating.  
{
	vec3 result;
	result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z);
	result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z);
	result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z);
	//THE TRANSLATION IS AN IMPLIED * 1! OF A 4 COMPONENT VECTOR! (x, y, z, 1)
	vec3 translate; translate.x = M.d[3][0];  translate.y = M.d[3][1]; translate.z = M.d[3][2];
	result = result + translate;
	return(result);
}

vec4 operator*(mat4x4 M, vec4 V)
{
	vec4 result;
	result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z) + (M.d[0][3] * V.w);
	result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z) + (M.d[1][3] * V.w);
	result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z) + (M.d[2][3] * V.w);
	result.w = (M.d[3][0] * V.x) + (M.d[3][1] * V.y) + (M.d[3][2] * V.z) + (M.d[3][3] * V.w);
	return(result);
}

void transpose_mat3(mat3x3* A)
{
	mat3x3 temp = (*A);
    A->d[0][1] = temp.d[1][0];  A->d[0][2] = temp.d[2][0];
	A->d[1][0] = temp.d[0][1]; 				   A->d[1][2] = temp.d[2][1];
	A->d[2][0] = temp.d[0][2]; A->d[2][1] = temp.d[1][2];
}

void transpose_mat4(mat4x4* A)
{
	mat4x4 temp = (*A);
    A->d[0][1] = temp.d[1][0];  A->d[0][2] = temp.d[2][0];	A->d[0][3] = temp.d[3][0];
	A->d[1][0] = temp.d[0][1]; 				   A->d[1][2] = temp.d[2][1]; A->d[1][3] = temp.d[3][1];
	A->d[2][0] = temp.d[0][2]; A->d[2][1] = temp.d[1][2];					A->d[2][3] = temp.d[3][2];
	A->d[3][0] = temp.d[0][3]; A->d[3][1] = temp.d[1][3];  A->d[3][2] = temp.d[2][3];
}

vec3 lerp_vec3(vec3 a, vec3 b, f32 progression)
{
    vec3 result;
    result.x = a.x + (b.x - a.x) * progression;
    result.y = a.y + (b.y - a.y) * progression;
    result.z = a.z + (b.z - a.z) * progression;
    return result;
} 

//NOTE: everything below here is all quaternion stuff.
//Because of this, it's entirely irrelevant to the game code. It is only staying in the codebase for knowledge reasons.
struct quaternion { f32 x; f32 y; f32 z; f32 r; };
#define generatequaternion(x, y, z, theta) {(f32)(x*sinf(theta/2)), (f32)(y*sinf(theta/2)), (f32)(z*sinf(theta/2)), (f32)(cosf(theta/2))};

quaternion nlerp(quaternion a, quaternion b, f32 progression)
{
    quaternion result;
    result.x = 0; result.y = 0; result.z = 0; result.r = 0;
    return result;
}

quaternion operator*(quaternion A, quaternion B)
{
    quaternion result;
    result.x = (A.r*B.x + B.r*A.x + A.x*B.x);
    result.y = (A.r*B.y + B.r*A.y + A.y*B.y);
    result.z = (A.r*B.z + B.r*A.z + A.z*B.z);
    result.r = (A.r*B.r - dot_vec3({A.x, A.y, A.z}, {B.x, B.y, B.z}));
    return result;
}

vec3 quaternionrotatevec3(quaternion Q, vec3 A)
{
    quaternion IQ;
    IQ.x = -Q.x;
    IQ.y = -Q.y;
    IQ.z = -Q.z;
    IQ.r = Q.r;
    quaternion AQ = {A.x, A.y, A.z, 0};
    quaternion qresult = Q*AQ*IQ;
    vec3 result;
    result.x = qresult.x;
    result.y = qresult.y;
    result.z = qresult.z;
    //NOTE: don't have to multiply W or R because W for A is going to be 0. So it'd be a waste of computing resources.
    return(result);
}