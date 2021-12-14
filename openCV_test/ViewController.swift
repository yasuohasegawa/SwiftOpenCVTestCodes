//
//  ViewController.swift
//  openCV_test
//
//  Created by Yasuo Hasegawa on 2021/12/02.
//

import UIKit
import AVFoundation

class ViewController: UIViewController, AVCaptureVideoDataOutputSampleBufferDelegate {
    // camera
    private var captureSession: AVCaptureSession = AVCaptureSession()
    private let videoDataOutput = AVCaptureVideoDataOutput()
    
    @IBOutlet weak var imageView: UIImageView!
    private let testImage = UIImageView()
    private var prevImage:UIImage? = UIImage()
    private let isMotionDetectTest = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        imageView.frame = view.frame
        addCameraInput()
        getFrames()
        captureSession.startRunning()
        //testOpenCV()
    }
    
    private func addCameraInput() {
        guard let device = AVCaptureDevice.DiscoverySession(
            deviceTypes: [.builtInWideAngleCamera, .builtInDualCamera, .builtInTrueDepthCamera],
            mediaType: .video,
            position: .back).devices.first else {
                fatalError("No back camera device found, please make sure to run SimpleLaneDetection in an iOS device and not a simulator")
        }
        let cameraInput = try! AVCaptureDeviceInput(device: device)
        self.captureSession.addInput(cameraInput)
    }
    
    private func getFrames() {
        videoDataOutput.videoSettings = [(kCVPixelBufferPixelFormatTypeKey as NSString) : NSNumber(value: kCVPixelFormatType_32BGRA)] as [String : Any]
        videoDataOutput.alwaysDiscardsLateVideoFrames = true
        videoDataOutput.setSampleBufferDelegate(self, queue: DispatchQueue(label: "camera.frame.processing.queue")) // set up the sample buffer delegate
        
        // Here, we can control the capture image resolution which we can control the image process quality.
        self.captureSession.sessionPreset = .iFrame1280x720 // set up the resolution
        
        self.captureSession.addOutput(videoDataOutput)
        guard let connection = self.videoDataOutput.connection(with: AVMediaType.video),
            connection.isVideoOrientationSupported else { return }
        connection.videoOrientation = .portrait

        connection.videoPreviewLayer?.videoGravity = .resizeAspectFill
    }
    
    func captureOutput( _ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        guard let  imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else { return }
        CVPixelBufferLockBaseAddress(imageBuffer, CVPixelBufferLockFlags.readOnly)
        let baseAddress = CVPixelBufferGetBaseAddress(imageBuffer)
        let bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer)
        let width = CVPixelBufferGetWidth(imageBuffer)
        let height = CVPixelBufferGetHeight(imageBuffer)
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        var bitmapInfo: UInt32 = CGBitmapInfo.byteOrder32Little.rawValue
        bitmapInfo |= CGImageAlphaInfo.premultipliedFirst.rawValue & CGBitmapInfo.alphaInfoMask.rawValue
        let context = CGContext(data: baseAddress, width: width, height: height, bitsPerComponent: 8, bytesPerRow: bytesPerRow, space: colorSpace, bitmapInfo: bitmapInfo)
        guard let quartzImage = context?.makeImage() else { return }
        CVPixelBufferUnlockBaseAddress(imageBuffer, CVPixelBufferLockFlags.readOnly)
        
        let image = UIImage(cgImage: quartzImage)
        
        var res:UIImage? = nil;
        if(!isMotionDetectTest){
            res = OpenCVMain.testDetect(image)
        } else {
            res = OpenCVMain.motionDetect(image, previousImage: prevImage)
            prevImage = UIImage(cgImage: quartzImage)
        }
        
        DispatchQueue.main.async {
            self.imageView.image = res
        }
    }
    
    func testOpenCV(){
        let screenW:CGFloat = view.frame.size.width
        let screenH:CGFloat = view.frame.size.height
                
        testImage.image = UIImage(named: "test")
        testImage.frame = CGRect(x:0, y:0, width:screenW, height:screenW)
        testImage.center = CGPoint(x:screenW/2, y:screenH/2)
        self.view.addSubview(testImage)
        
        testImage.image = OpenCVMain.blur(testImage.image)
    }

}

