#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"
#include "cinder/Sphere.h"
#include <utility>
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderProject3App : public AppBasic {
  public:
	  void setup();
	  void prepareSettings(Settings *settings);
	  void mouseDown(MouseEvent event);
	  void mouseMove(MouseEvent event);
	  void mouseDrag(MouseEvent event);
	  void drawGrid(float size, float step);

	  void draw();
	  void load();
	  void update(); 
	  gl::TextureFontRef	mFont;
	  bool					mUseCairo;
	  svg::DocRef				mDoc;
	  gl::Texture				mTex;
	  svg::Node 			*mCurrentCountry;
	  svg::Style style ;	  
	  svg::Paint     pain;
	  Anim<float>			mCurrentCountryAlpha;
};


gl::Texture renderSvgToTexture(svg::DocRef doc, Vec2i size)
{
	cairo::SurfaceImage srf(size.x, size.y, false);
	cairo::Context ctx(srf);
	ctx.render(*doc);
	srf.flush();
	return gl::Texture(srf.getSurface());
}

void CinderProject3App::prepareSettings(Settings *settings)
{
	settings->setResizable(true);
	settings->setWindowSize(1280, 720);
}




void CinderProject3App::setup()
{
	mDoc =  svg::Doc::create(loadAsset("Europe.svg"));
	style = mDoc->calcInheritedStyle();
	style.setStrokeWidth(30.0f);

	mTex = renderSvgToTexture(mDoc, getWindowSize());
	mFont = gl::TextureFont::create(Font(loadAsset("Dosis-Medium.ttf"), 36));
	mCurrentCountry = 0;
 
}
 


void CinderProject3App::load()
{

}
void CinderProject3App::mouseMove(MouseEvent event)
{
	svg::Node *newNode = mDoc->nodeUnderPoint(event.getPos());
	
	mCurrentCountry = newNode;
	auto style = const_cast<svg::Style*>(&mCurrentCountry->getStyle());
	style->setStrokeOpacity(1) ;
	
	
	// if the current node has no name just set it to NULL
	if (mCurrentCountry && mCurrentCountry->getId().empty())
		mCurrentCountry = NULL;
}
void CinderProject3App::mouseDown( MouseEvent event )
{
 }
void CinderProject3App::mouseDrag(MouseEvent event)
{
 
}
void CinderProject3App::update()
{
	

}
void CinderProject3App::drawGrid(float size, float step)
{
	gl::color(Color(0.7f, 0.7f, 0.7f));
	//draw grid
	for (float i = 0; i <= size; i += step) {
		gl::drawLine(Vec3f(400+i,700,-size), Vec3f(i, 0.f, size));
	}
}
void CinderProject3App::draw()
{


	gl::clear(Color::gray(0.5f));
	// turn on z-buffering
	//gl::enableDepthRead();
	//gl::enableDepthWrite();
	//gl::enableAlphaBlending();
	if (mTex) {
		gl::color(Color::white());
	//gl::draw(*mDoc);
		drawGrid(800.0f, 80.0f);

	}

  //gl::drawCube(Vec3f( 40, 30.f,40) ,Vec3f(40.f, 40.f, 40.f));
  gl::drawString ("Drag & Drop an SVG file", Vec2f(0, 45));
  gl::drawString ("Click to toggle between Cairo & OpenGL",  Vec2f(0, 20));
 
	if (mCurrentCountry) {

		string countryName = mCurrentCountry->getId();
		// draw the outline
		gl::color(ColorA(244, 0, 0, 1));
		gl::drawSolid(mCurrentCountry->getShapeAbsolute());
		gl::draw(mCurrentCountry->getShapeAbsolute());


	//gl::draw(mCurrentCountry->getShapeAbsolute());
	
		gl::drawString("Click to toggle between Cairo & OpenGL", Vec2f(0, 30));
		gl::color(ColorA(244, 0, 0, 1));
		mFont->drawString(countryName, Vec2f(0, 30));
	}
 


}


CINDER_APP_BASIC( CinderProject3App, RendererGl )
