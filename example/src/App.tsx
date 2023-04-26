import React from 'react';
import { StyleSheet, View, Text, Pressable, Platform } from 'react-native';
import { launchImageLibrary } from 'react-native-image-picker';
import { getImageMetadata, ImageMetadata } from 'react-native-image-metadata';

const App = () => {
  const [metadata, setMetadata] = React.useState<ImageMetadata | null>(null);

  const selectAndGetMetadata = async () => {
    const result = await launchImageLibrary({
      mediaType: 'photo',
      selectionLimit: 1,
    });

    if (result.didCancel) return;
    if (result.errorCode) return;
    if (!result.assets || result.assets.length === 0) return;

    const asset = result.assets[0];
    if (!asset || !asset.uri) return;

    console.log('Selected image:');
    console.log(asset);

    const normalizedUri =
      Platform.OS === 'ios' ? asset.uri.replace('file://', '') : asset.uri;
    console.log(normalizedUri);
    try {
      const meta = getImageMetadata(normalizedUri);
      setMetadata(meta);
    } catch (error) {
      setMetadata(null);
      if (error instanceof Error) {
        console.error(error);
        console.error(error.message);
      }
    }
  };

  return (
    <View style={styles.container}>
      <Pressable onPress={selectAndGetMetadata} style={styles.chooseButton}>
        <Text>Choose an image</Text>
      </Pressable>

      {metadata && (
        <Text style={styles.metadata}>
          Metadata: {JSON.stringify(metadata)}
        </Text>
      )}
    </View>
  );
};

export default App;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: 'white',
    alignItems: 'center',
    justifyContent: 'center',
  },
  chooseButton: {
    padding: 12,
    backgroundColor: 'yellow',
  },
  metadata: {
    marginTop: 12,
    paddingHorizontal: 12,
    fontSize: 18,
  },
});
