
RenderObject* makeRoadRenderObject(RoadSegment* road, RenderObject* o)
{
	const int Reso = 20;
	Vector ver[2*(Reso+1)];
	Vector nor[2*(Reso+1)];
	float tex[4*(Reso+1)];
	RenderItem* items[4] = {new RenderItem(), new RenderItem(), new RenderItem(), new RenderItem()};
	for(int j=0; j<4; j++){
		for(int i=0; i<=Reso; i++){
			float t = float(i)/float(Reso);
			Vector p = road->point(t);
			float sign = (j%2 == 0) ? 1.f : -1.f;
			Vector R = road->right(t);
			Vector N = unit(cross(R, road->velocity(t)));
			Vector r = j/2 ? N : R;
			Vector n = (j/2 ? -R : N) * sign;
			float W = j/2 ? RoadHeight : RoadWidth;
			float H = j/2 ? RoadWidth : RoadHeight;
			nor[i*2+0] = nor[i*2+1] = n;
			ver[i*2+1] = p + r * sign * W/2 + n * H/2;
			ver[i*2+0] = p - r * sign * W/2 + n * H/2;
			tex[i*4+0] = float(i);
			tex[i*4+1] = +W/2 * sign;
			tex[i*4+2] = float(i);
			tex[i*4+3] = -W/2 * sign;
		}
		glGenBuffers(1, &items[j]->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, items[j]->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW_ARB);
		glGenBuffers(1, &items[j]->texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, items[j]->texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW_ARB);
		glGenBuffers(1, &items[j]->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, items[j]->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(nor), nor, GL_STATIC_DRAW_ARB);
		items[j]->primType = GL_QUAD_STRIP;
		items[j]->vertexCount = sizeof(ver)/sizeof(ver[0]);
		items[j]->color = 0xffc0c0c0;
		//int clrs[] = {0xffFF8080, 0xff80FF80, 0xff8080ff, 0xffFFff80};
		//items[j]->color = clrs[j];
	}
	items[0]->next = items[1]; items[1]->next = items[2]; items[2]->next = items[3];
	o->items = items[0];
	return o;
}
