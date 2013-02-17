#pragma once
// Based on "Computation of Rotation Minimizing Frames" Wang et al. 2008

#include <vector>
typedef unsigned int uint;

#define ZERO_NORM 1e-10

class RMF{
public:
	RMF(){}

	RMF(const std::vector<Vec3d> & fromPoints)
	{
		point = fromPoints;

		compute();
	}

	void compute()
	{
		// Reset computation
		std::vector<Vec3d> tangent;

		// Estimate tangents
		for(uint i = 0; i < point.size() - 1; i++){
			Vec3d t = point[i+1] - point[i];
			if(i > 0 && t.norm() < ZERO_NORM) 
				tangent.push_back(tangent.back());
			else
				tangent.push_back((t).normalized());
		}
		tangent.push_back(tangent.back());

		// First frame
		Frame firstFrame = Frame::fromT(tangent.front());

		U.clear();
		U.push_back( firstFrame );

		// Double reflection method: compute rotation minimizing frames
		for(uint i = 0; i < point.size() - 1; i++)
		{
			Vec3d ri = U.back().r, ti = U.back().t, tj = tangent[i+1];

			/*1 */ Vec3d v1 = point[i+1] - point[i]; if(v1.norm() < ZERO_NORM){ U.push_back(U.back()); continue; }
			/*2 */ double c1 = dot(v1,v1);
			/*3 */ Vec3d rLi = ri - (2.0 / c1) * dot(v1, ri) * v1;
			/*4 */ Vec3d tLi = ti - (2.0 / c1) * dot(v1, ti) * v1;
			/*5 */ Vec3d v2 = tj - tLi;
			/*6 */ double c2 = dot(v2,v2);
			/*7 */ Vec3d rj = rLi - (2.0 / c2) * dot(v2, rLi) * v2;
			/*8 */ Vec3d sj = cross(tj,rj);

			U.push_back(Frame::fromST(sj, tj));
		}

		// RMF Visualization
		for(int i = 0; i < (int)point.size(); i++)
			U[i].center = point[i];
	}

	inline uint count() { return point.size(); }

	class Frame{ 
	public:
		Vec3d r, s, t;
		Vec3d center; // optional

		Frame(){ r = Vec3d(1,0,0); s = Vec3d(0,1,0); t = Vec3d(0,0,1); }
		Frame(const Vec3d& R, const Vec3d& S, const Vec3d& T) { r = R; s = S; t = T; normalize(); }

		static Frame fromTR(const Vec3d& T, const Vec3d& R) { return Frame(R, cross(T,R), T); }
		static Frame fromRS(const Vec3d& R, const Vec3d& S) { return Frame(R, S, cross(R,S)); }
		static Frame fromST(const Vec3d& S, const Vec3d& T) { return Frame(cross(S,T), S, T); }
		static Frame fromT(const Vec3d& T) { Vec3d R = orthogonalVector(T.normalized()).normalized(); return fromTR(T,R); }

		static Vec3d orthogonalVector(const Vec3d& n) {
			if ((abs(n.y()) >= 0.9 * abs(n.x())) && abs(n.z()) >= 0.9 * abs(n.x())) return Vec3d(0.0, -n.z(), n.y());
			else if ( abs(n.x()) >= 0.9 * abs(n.y()) && abs(n.z()) >= 0.9 * abs(n.y()) ) return Vec3d(-n.z(), 0.0, n.x());
			else return Vec3d(-n.y(), n.x(), 0.0);
		}

		void normalize() { r.normalize(); s.normalize(); t.normalize(); } ;
	};

	inline Frame frameAt(double t){
		return U[ (qRanged(0.0, t, 1.0) * (U.size() - 1)) ];
	}

	std::vector<Vec3d> point;
	std::vector<Frame> U;
};
