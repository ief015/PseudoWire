#pragma once

#include "../Vector.h"

#include <vector>

namespace sys
{
namespace physics
{
	struct Contact
	{
		bool IsTouching;
		bool WillTouch;
		Vector<float> TouchTranslation;
		Vector<float> WillTouchTranslation;
	};

	class Fixture
	{
	public:
		Fixture(const std::vector< sys::Vector<float> > & Points);
		~Fixture();

		inline Vector<float> & GetPoint(int idx) { return points[idx]; }
		inline Vector<float> & GetEdge(int idx) { return edges[idx]; }
		inline unsigned int GetPointCount() { return points.size(); }
		inline unsigned int GetEdgeCount() { return edges.size(); }

		void UpdateEdges();
		Vector<float> GetAABB();
		Vector<float> FindCenter();
		inline Vector<float> Position() { return pos; }
		void SetPosition(const Vector<float> & newPos);
		void Move(const Vector<float> & offset);
		float Rotation();
		void SetRotation(float rads);

		bool Colliding(Fixture* OtherFixture);
		Contact Colliding(const Vector<float> & Velocity, Fixture* OtherFixture);

	protected:
		std::vector< Vector<float> > points;
		std::vector< Vector<float> > edges;

		Vector<float> pos;

		static float IntervalDistance(float minA, float maxA, float minB, float maxB);
		void ProjectPolygon(const Vector<float> & axis, float & min, float & max);
		/*
			private List<Point> points = new List<Point>();
			private List<Point> edges = new List<Point>();
			public List<Point> Points { get { return points; } }
			public List<Point> Edges { get { return edges; } }

			public void UpdateEdges()
			{
				Point p1;
				Point p2;
				edges.Clear();
				for (int i = 0; i < points.Count; i++)
				{
					p1 = points[i];
					if (i + 1 >= points.Count)
						p2 = points[0];
					else
						p2 = points[i + 1];
					edges.Add(p2 - p1);
				}
			}

			public Point GetAABB()
			{
				Point r = new Point();
				foreach (Point p in points)
				{
					if (p.X > r.X) r.X = p.X;
					if (p.Y > r.Y) r.Y = p.Y;
				}
				r -= this.Position;
				return r;
			}

			internal Point centerOfGravity;
			public Point CenterOfGravity
			{
				get { return centerOfGravity; }
			}

			private Point center;
			private bool UPDATECENTER = true;
			public Point Center
			{
				get
				{
					if (UPDATECENTER)
					{
						float totalX = 0;
						float totalY = 0;
						for (int i = 0; i < points.Count; i++)
						{
							totalX += points[i].X;
							totalY += points[i].Y;
						}
						center = new Point(totalX / (float)points.Count, totalY / (float)points.Count);
					}

					return center;
				}
			}

			private Point pos;
			public Point Position
			{
				get { return pos; }
				set
				{
					for (int i = 0; i < points.Count; i++)
						points[i] += value - pos;
					centerOfGravity += value - pos;

					UPDATECENTER = true;
					pos = value;
				}
			}

			public void PositionSetX(float x)
			{
				this.Position = new Point(x, pos.Y);
			}
			public void PositionSetY(float y)
			{
				this.Position = new Point(pos.X, y);
			}

			private float rot;
			public float Rotation
			{
				get { return rot; }
				set
				{
					if (value == 0f) return;

					float rotInc = value - rot;
					Point origin = this.CenterOfGravity;
					float sin = (float)(Math.Sin(Math.PI / 180f * rotInc));
					float cos = (float)(Math.Cos(Math.PI / 180f * rotInc));
					//{
					//	Point o = new Point(2, 2);
					//	Point v = new Point(15, 5);
					//	v -= o;
					//	v = new Point(
					//		(v.X * cos) - (v.Y * sin),
					//		(v.X * sin) + (v.Y * cos)
					//	);
					//	v += o;
					//}
					for (int i = 0; i < points.Count; i++)
					{
						points[i] -= origin;
						points[i] = new Point(
							(points[i].X * cos) - (points[i].Y * sin),
							(points[i].X * sin) + (points[i].Y * cos)
						);
						points[i] += origin;
					}
					this.UpdateEdges();
					UPDATECENTER = true;

					rot = value;
					while (true)
					{
						if (rot < 0f)
							rot += 360f;
						else if (rot >= 360f)
							rot -= 360f;
						else
							break;
					}
				}
			}

			public override string ToString()
			{
				string result = "";

				for (int i = 0; i < points.Count; i++)
				{
					if (result != "") result += " ";
					else result = "{ ";
					result += "{" + points[i].ToString(true) + "}";
				}
				result += " }";

				return result;
			}

			// Check if polygon A is going to collide with polygon B for the given velocity
			public PolygonCollisionResult PolygonCollision(Point thisVelocity, CollisionModel OtherPolygon)
			{
				PolygonCollisionResult result = new PolygonCollisionResult();
				result.Intersect = true;
				result.WillIntersect = true;
				result.Axis = new Point();

				float minIntervalDistance = float.PositiveInfinity;
				Point translationAxis = new Point();
				Point edge;

				// Loop through all the edges of both polygons
				for (int edgeIndex = 0; edgeIndex < this.Edges.Count + OtherPolygon.Edges.Count; edgeIndex++)
				{
					if (edgeIndex < this.Edges.Count)
						edge = this.Edges[edgeIndex];
					else
						edge = OtherPolygon.Edges[edgeIndex - this.Edges.Count];

					// ===== 1. Find if the polygons are currently intersecting =====

					// Find the axis perpendicular to the current edge
					Point axis = new Point(-edge.Y, edge.X);
					axis.Normalize();

					// Find the projection of the polygon on the current axis
					float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
					this.ProjectPolygon(axis, ref minA, ref maxA);
					OtherPolygon.ProjectPolygon(axis, ref minB, ref maxB);

					// Check if the polygon projections are currentlty intersecting
					if (IntervalDistance(minA, maxA, minB, maxB) > 0) result.Intersect = false;

					// ===== 2. Now find if the polygons *will* intersect =====

					// Project the velocity on the current axis
					float velocityProjection = axis.DotProduct(thisVelocity);

					// Get the projection of polygon A during the movement
					if (velocityProjection < 0)
						minA += velocityProjection;
					else
						maxA += velocityProjection;

					// Do the same test as above for the new projection
					float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
					if (intervalDistance > 0) result.WillIntersect = false;

					// If the polygons are not intersecting and won't intersect, exit the loop
					if (!result.Intersect && !result.WillIntersect) break;
					// Check if the current interval distance is the minimum one. If so store
					// the interval distance and the current distance.
					// This will be used to calculate the minimum translation vector
					intervalDistance = Math.Abs(intervalDistance);
					if (intervalDistance < minIntervalDistance)
					{
						minIntervalDistance = intervalDistance;
						translationAxis = axis;

						Point d = this.Center - OtherPolygon.Center;
						if (d.DotProduct(translationAxis) < 0f) translationAxis = -translationAxis;
					}
				}

				// The minimum translation vector can be used to push the polygons apart.
				// First moves the polygons by their velocity
				// then move polygonA by MinimumTranslationVector.
				if (result.WillIntersect)
					result.WillMinimumTranslationVector = translationAxis * minIntervalDistance;
				if (result.Intersect)
				{
					result.MinimumTranslationVector = translationAxis * minIntervalDistance;

					this.centerOfGravity = this.Center - result.MinimumTranslationVector;
					//this.centerOfGravity = result.Axis;
					//this.RotationVelocity = (float)Math.PI / 180f * -(this.Center).DotProduct(result.MinimumTranslationVector);
					//this.RotationVelocity = -(this.centerOfGravity).DotProduct(result.MinimumTranslationVector) / 180;
					//this.RotationVelocity = (this.centerOfGravity.GetNormalized() / result.Axis).Magnitude;
					//this.RotationVelocity = (result.MinimumTranslationVector.Magnitude / 2f) * Math.Sign(result.MinimumTranslationVector.X / 2f);
				}
				else
				{
					this.centerOfGravity = this.Center;
				}

				return result;
			}

			public PolygonCollisionResult PolygonCollision(CollisionModel OtherPolygon)
			{
				PolygonCollisionResult result = new PolygonCollisionResult();
				result.Intersect = true;
				result.WillIntersect = false;
				result.MinimumTranslationVector = new Point();
				result.WillMinimumTranslationVector = new Point();

				Point edge;

				// Loop through all the edges of both polygons
				for (int edgeIndex = 0; edgeIndex < this.Edges.Count + OtherPolygon.Edges.Count; edgeIndex++)
				{
					if (edgeIndex < this.Edges.Count)
						edge = this.Edges[edgeIndex];
					else
						edge = OtherPolygon.Edges[edgeIndex - this.Edges.Count];

					// ===== 1. Find if the polygons are currently intersecting =====

					// Find the axis perpendicular to the current edge
					Point axis = new Point(-edge.Y, edge.X);
					axis.Normalize();

					// Find the projection of the polygon on the current axis
					float minA = 0f, minB = 0f, maxA = 0f, maxB = 0f;
					this.ProjectPolygon(axis, ref minA, ref maxA);
					OtherPolygon.ProjectPolygon(axis, ref minB, ref maxB);

					// Check if the polygon projections are currentlty intersecting
					if (IntervalDistance(minA, maxA, minB, maxB) > 0) result.Intersect = false;

					if (!result.Intersect) break;
				}

				return result;
			}

			// Calculate the distance between [minA, maxA] and [minB, maxB]
			// The distance will be negative if the intervals overlap
			private static float IntervalDistance(float minA, float maxA, float minB, float maxB)
			{
				if (minA < minB)
					return minB - maxA;
				else
					return minA - maxB;
			}

			// Calculate the projection of a polygon on an axis and returns it as a [min, max] interval
			private void ProjectPolygon(Point axis, ref float min, ref float max)
			{
				// To project a point on an axis use the dot product
				float d = axis.DotProduct(this.Points[0]);
				min = d;
				max = d;
				for (int i = 0; i < this.Points.Count; i++)
				{
					d = this.Points[i].DotProduct(axis);
					if (d < min)
						min = d;
					else if (d > max)
						max = d;
				}
			}
		*/
	};
}
}