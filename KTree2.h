
typedef struct kdNode2D
{
	kdNode2D(pPoint2D pointList, int pointLength, int depth = 0);

	~kdNode2D()
	{
		for(int i=0; i<2; ++i)
			delete sons[i];
	}

	/* Leave depth alone for outside code! */
	unsigned nearest(const Point2D &point, int depth = 0);

	union {
		struct {
			kdNode2D* left;
			kdNode2D* right;
		};

		kdNode2D* sons[2];
	};

	Point2D p;

} kdNode2D;