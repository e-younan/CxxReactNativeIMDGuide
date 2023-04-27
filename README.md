# react-native-image-metadata

A library for extracting PNG metadata in React Native, using C++ Turbo Native Modules. This repository is the outcome of my Medium Post, [Building a React Native C++ Turbo Native Module](https://codeherence.medium.com/building-a-react-native-c-turbo-native-module-part-1-908afd03c0f8).


## Usage


```js
import { getImageMetadata } from 'react-native-image-metadata';

// ...

const result = getImageMetadata("/path/to/somePNG.png");
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
