//
//  SampleFacade.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 8/14/12.
//
//

#import "SampleFacade.h"
#import "NSString+StdString.h"
#import "UIImage2OpenCV.h"

@interface SampleFacade()
{
    NSString * m_title;
    NSString * m_description;
    UIImage  * m_smallIcon;
    UIImage  * m_largeIcon;
}

@end


@implementation SampleFacade
@synthesize sample = _sample;

- (id) initWithSample:(SampleBase*) s
{
    assert(s);
    
    if (self = [super init])
    {
        _sample = s;
    }
    
    return self;
}

- (NSString *) title
{
    if (!m_title)
    {
        m_title = [NSString stringWithCString:self.sample->getName().c_str() encoding:NSASCIIStringEncoding];
    }
    
    return m_title;
}

- (NSString *) description
{
    if (!m_description)
    {
        m_description = [NSString stringWithCString:self.sample->getDescription().c_str() encoding:NSASCIIStringEncoding];
    }
    
    return m_description;
}


- (UIImage*)   smallIcon
{
    if (!m_smallIcon)
    {
        if (self.sample->hasIcon())
        {
            NSString * iconStr = [NSString stringWithStdString:self.sample->getSampleIcon()];
            m_smallIcon = [[UIImage imageNamed:iconStr] thumbnailWithSize:80];
        }
        else
        {
            UIImage * srcImage = [UIImage imageNamed:@"DefaultSampleIcon.png"];
            m_smallIcon = [self processFrame:[srcImage thumbnailWithSize:80]];
        }
    }
    
    return m_smallIcon;
}

- (UIImage*)   largeIcon
{
    if (!m_largeIcon)
    {
        if (self.sample->hasIcon())
        {
            NSString * iconStr = [NSString stringWithStdString:self.sample->getSampleIcon()];
            m_largeIcon = [UIImage imageNamed:iconStr];
        }
        else
        {
            UIImage * src = [UIImage imageNamed:@"DefaultSampleIcon.png"];
            assert(src);

            m_largeIcon = [self processFrame:src];
        }
    }
    
    return m_largeIcon;
}

- (bool) processFrame:(const cv::Mat&) inputFrame into:(cv::Mat&) outputFrame
{
    return self.sample->processFrame(inputFrame, outputFrame);
}

- (UIImage*) processFrame:(UIImage*) source
{
    cv::Mat inputImage = [source toMat];
    cv::Mat outputImage;
    
    self.sample->processFrame(inputImage, outputImage);
    UIImage * result = [UIImage imageWithMat:outputImage andImageOrientation:[source imageOrientation]];
    return result;
}

- (NSString *) friendlyName
{
    return [NSString stringWithCString:self.sample->getUserFriendlyName().c_str() encoding:NSASCIIStringEncoding];
}


@end
