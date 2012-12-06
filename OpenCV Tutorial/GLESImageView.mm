//
//  GLESView.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "GLESImageView.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "FPSCalculator.h"

@interface GLESImageView()
{
  // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
  GLuint defaultFramebuffer, colorRenderbuffer;
  GLuint backgroundTextureId;
  GLint framebufferWidth, framebufferHeight;
  
}
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, strong) FPSCalculator * fpsCalculator;
@property (nonatomic, strong) UILabel *fpsLabel;

@end

@implementation GLESImageView
@synthesize fpsLabel;
@synthesize context;
@synthesize fpsCalculator;

// You must implement this method
+ (Class)layerClass
{
  return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
      CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
      
      eaglLayer.opaque = TRUE;
      eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                      [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                      kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                      nil];
      
      self.fpsCalculator = [[FPSCalculator alloc] init];
      
      self.fpsLabel = [[UILabel alloc] initWithFrame:CGRectMake(5, 5, 100, 100)];
      self.fpsLabel.text = @"XXX FPS";
      self.fpsLabel.textColor = [UIColor greenColor];
      self.fpsLabel.backgroundColor = [UIColor clearColor];
      self.fpsLabel.numberOfLines = 1;
      
      [self.fpsLabel sizeToFit];
      
      [self addSubview:self.fpsLabel];
            
      [self initContext];
    }
    return self;
}

- (void)dealloc
{
  [self deleteFramebuffer]; 
  
  if ([EAGLContext currentContext] == context)
    [EAGLContext setCurrentContext:nil];
  
}

- (void)setContext:(EAGLContext *)newContext
{
  if (context != newContext)
  {
    [self deleteFramebuffer];
    
    context = newContext;
    
    [EAGLContext setCurrentContext:nil];
  }
}

- (void)createFramebuffer
{
  if (context && !defaultFramebuffer)
  {
    [EAGLContext setCurrentContext:context];
    
    // Create default framebuffer object.
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    // Create color render buffer and allocate backing store.
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    
    //glClearColor(0, 0, 0, 0);
    NSLog(@"Framebuffer created %d x %d", framebufferWidth, framebufferHeight);
  }
}

- (void)deleteFramebuffer
{
  if (context)
  {
    [EAGLContext setCurrentContext:context];
    
    if (defaultFramebuffer) {
      glDeleteFramebuffers(1, &defaultFramebuffer);
      defaultFramebuffer = 0;
    }
    
    if (colorRenderbuffer) {
      glDeleteRenderbuffers(1, &colorRenderbuffer);
      colorRenderbuffer = 0;
    }
    
    NSLog(@"Framebuffer deleted");
    
  }
}

- (void)setFramebuffer
{
  if (context) {
    [EAGLContext setCurrentContext:context];
    
    if (!defaultFramebuffer)
      [self createFramebuffer];
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
  }
}

- (BOOL)presentFramebuffer
{
  BOOL success = FALSE;
  
  if (context) {
    [EAGLContext setCurrentContext:context];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    success = [context presentRenderbuffer:GL_RENDERBUFFER];
  }
  
  return success;
}

- (void)layoutSubviews
{
  [super layoutSubviews];
  
  // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
  [self deleteFramebuffer];
}

- (void)initContext
{
  EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
  
  if (!aContext)
    NSLog(@"Failed to create ES context");
  else if (![EAGLContext setCurrentContext:aContext])
    NSLog(@"Failed to set ES context current");
  
  [self setContext:aContext];
  [self setFramebuffer];
  
  glGenTextures(1, &backgroundTextureId);
  glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // This is necessary for non-power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

- (UIViewController *)viewController {
  UIResponder *responder = self;
  while (![responder isKindOfClass:[UIViewController class]]) {
    responder = [responder nextResponder];
    if (nil == responder) {
      break;
    }
  }
  return (UIViewController *)responder;
}

- (void)drawFrame:(const cv::Mat&) bgraFrame
{
  [self setFramebuffer];  
  [self.fpsCalculator putTimeMark];
  
  self.fpsLabel.text = [self.fpsCalculator getFPSAsText];
  
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  //glPixelStorei(GL_PACK_ROW_LENGTH, (size_t)bgraFrame.step);
  glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bgraFrame.cols, bgraFrame.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, bgraFrame.data);
  
  UIInterfaceOrientation uiOrientation = [[self viewController] interfaceOrientation];
  
  GLfloat * textureVertices;
  static GLfloat textureVerticesPortrait[] =
  {
    1, 1,   1, 0,
    0, 1,   0, 0
  };  
  
  static GLfloat textureVerticesPortraitUpsideDown[] =
  {
    0, 0,   0, 1,
    1, 0,   1, 1
  };
  
  static GLfloat textureVerticesLandscapeLeft[] =
  {
    1, 0,   0, 0,
    1, 1,   0, 1
  };  
  
  static GLfloat textureVerticesLandscapeRight[] =
  {
    0, 1,   1, 1,
    0, 0,   1, 0
  }; 
  
  switch (uiOrientation)
  {
    case UIInterfaceOrientationPortrait:
      textureVertices = textureVerticesPortrait;
      break;

    case UIInterfaceOrientationPortraitUpsideDown:
      textureVertices = textureVerticesPortraitUpsideDown;
      break;

    case UIInterfaceOrientationLandscapeLeft:
      textureVertices = textureVerticesLandscapeLeft;
      break;

    case UIInterfaceOrientationLandscapeRight:
    default:
      textureVertices = textureVerticesLandscapeRight;
      break;
  };
  
  static const GLfloat squareVertices[] =
  {
    -1, -1,
    +1, -1,
    -1, +1,
    +1, +1
  };
  
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glDisable(GL_COLOR_MATERIAL);
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
  
  // Update attribute values.
  glVertexPointer(2, GL_FLOAT, 0, squareVertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, textureVertices);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glColor4f(1,1,1,1);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_TEXTURE_2D);
  
  bool ok = [self presentFramebuffer];
  assert(ok);
}

@end
