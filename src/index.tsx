import { IMD } from './NativeImageMetadata';
import type { ImageMetadata } from './NativeImageMetadata';

export const getImageMetadata = (path: string): ImageMetadata => {
  return IMD.getImageMetadata(path);
};

export type { ImageMetadata };
