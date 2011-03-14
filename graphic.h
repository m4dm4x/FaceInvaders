#ifndef GRAPHIC_H_
#define GRAPHIC_H_

class Graphic {
public:
	virtual ~Graphic(void) {};
	virtual void draw(void) =0;
	virtual void update(void)=0;
};

#endif /* GRAPHIC_H_ */
