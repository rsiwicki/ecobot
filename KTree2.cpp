static int cmpX(const void* a, const void* b)
{
	return (*(pPoint2D)a).x - (*(pPoint2D)b).x;
}

static int cmpY(const void* a, const void* b)
{
	return (*(pPoint2D)a).y - (*(pPoint2D)b).y;
}

kdNode2D::kdNode2D(pPoint2D pointList, int pointLength, int depth)
{
	if(pointLength == 1) {
		left	= NULL;
		right	= NULL;
		p		= *pointList;
		return;
	}

		// Odd depth = Y, even depth = X
	if(depth & 1)
		qsort(pointList, pointLength, sizeof(Point2D), cmpY);
	else
		qsort(pointList, pointLength, sizeof(Point2D), cmpX);

	const int halfLength = pointLength >> 1;
	p = pointList[halfLength];
	for(int i=0; i<2; ++i)
		sons[i] = new kdNode2D(pointList + (i*halfLength), halfLength, depth + 1);
}

unsigned kdNode2D::nearest(const Point2D &point, int depth)
{
	/* End of tree. */
	if(!left || !right)   // We assume if left != NULL, then right != NULL (see ctor)
	{
		Point2D r = p;
		for(int i=0; i<2; ++i)
			r[i] -= point[i];

		return r.dot(r);
	}

	const int tmp = p[depth] - point[depth];
	const int side = tmp < 0; /* Prefer the left. */

	/* Switch depth. */
	depth ^= 1;

	/* Search the near side of the tree. */
	int leftDist = sons[side]->nearest(point, depth);

	/* Radius intersects a kd tree boundary? */
	if(leftDist < (tmp * tmp))
	{
		/* No; this is the nearest point on this side. */
		return leftDist;
	}

	/* Yes; look at the points on the other side. */
	return min(leftDist, sons[side ^ 1]->nearest(point, depth));
}