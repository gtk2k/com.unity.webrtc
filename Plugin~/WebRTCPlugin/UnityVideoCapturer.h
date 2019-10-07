#pragma once

namespace WebRTC
{
    class UnityEncoder;
    class UnityVideoCapturer : public rtc::AdaptedVideoTrackSource
    {
    public:
        UnityVideoCapturer(UnityEncoder* pEncoder, int _width, int _height);
        void EncodeVideoData();
        // Returns true if the capturer is screencasting. This can be used to
        // implement screencast specific behavior.
        virtual bool is_screencast() const override
        {
            return false;
        }

        virtual absl::optional<bool> needs_denoising() const override
        {
            return false;
        }

        webrtc::MediaSourceInterface::SourceState state() const override
        {
            return SourceState::kLive;
        }
        bool remote() const override
        {
            return false;
        }

        void StartEncoder();
        void InitializeEncoder();
        void SetKeyFrame();
        void SetRate(uint32 rate);
        void CaptureFrame(std::vector<uint8>& data);
        bool CaptureStarted() { return captureStarted; }
    public:
        UnityFrameBuffer* unityRT = nullptr;
    private:
        UnityEncoder* nvEncoder;

        //just fake info
        int32 width;
        int32 height;
        const int32 framerate = 60;

        bool captureStarted = false;
        bool captureStopped = false;

    };

    class FrameBuffer : public webrtc::VideoFrameBuffer
    {
    public:
        std::vector<uint8>& buffer;

        FrameBuffer(int width, int height, std::vector<uint8>& data) : frameWidth(width), frameHeight(height), buffer(data) {}

        //webrtc::VideoFrameBuffer pure virtual functions
        // This function specifies in what pixel format the data is stored in.
        virtual Type type() const override
        {
            //fake I420 to avoid ToI420() being called
            return Type::kI420;
        }
        // The resolution of the frame in pixels. For formats where some planes are
        // subsampled, this is the highest-resolution plane.
        virtual int width() const override
        {
            return frameWidth;
        }
        virtual int height() const override
        {
            return frameHeight;
        }
        // Returns a memory-backed frame buffer in I420 format. If the pixel data is
        // in another format, a conversion will take place. All implementations must
        // provide a fallback to I420 for compatibility with e.g. the internal WebRTC
        // software encoders.
        virtual rtc::scoped_refptr<webrtc::I420BufferInterface> ToI420() override
        {
            return nullptr;
        }

    private:
        int frameWidth;
        int frameHeight;
    };
}
