import { NativeModules } from 'react-native';

declare global {
  function nativeCallSyncHook(): unknown;
  var __IMDProxy: object | undefined;
}

if (global.__IMDProxy == null) {
  const IMDModule = NativeModules.IMD;

  if (IMDModule == null) {
    throw new Error(
      'Base react-native-image-metadata module not found. Maybe try rebuilding the app.'
    );
  }

  // Check if we are running on-device (JSI)
  if (global.nativeCallSyncHook == null || IMDModule.install == null) {
    throw new Error(
      'Failed to install react-native-image-metadata: React Native is not running on-device. ImageMetadata can only be used when synchronous method invocations (JSI) are possible. If you are using a remote debugger (e.g. Chrome), switch to an on-device debugger (e.g. Flipper) instead.'
    );
  }

  // Call the synchronous blocking install() function
  const result = IMDModule.install();
  if (result !== true) {
    throw new Error(
      `Failed to install react-native-image-metadata: The native ImageMetadata Module could not be installed! Looks like something went wrong when installing JSI bindings: ${result}`
    );
  }

  // Check again if the constructor now exists. If not, throw an error.
  if (global.__IMDProxy == null) {
    throw new Error(
      'Failed to install react-native-image-metadata, the native initializer function does not exist. Are you trying to use ImageMetadata from different JS Runtimes?'
    );
  }
}

interface IIMD {
  getImageMetadata: (filePath: string) => ImageMetadata;
}

const proxy = global.__IMDProxy;
export const IMD = proxy as IIMD;

export type ImageMetadata = {
  /**
   * The width of the image.
   */
  width: number;
  /**
   * The height of the image.
   */
  height: number;
  /**
   * The size of the image in bytes.
   */
  size: number;
  /**
   * The bit depth of the image.
   */
  bitDepth: number;
  /**
   * The color type of the image.
   */
  colorType: number;
  /**
   * The interlace method of the image.
   */
  interlaceMethod: number;
  /**
   * The compression method of the image.
   */
  compressionMethod: number;
  /**
   * The filter method of the image.
   */
  filterMethod: number;
};
