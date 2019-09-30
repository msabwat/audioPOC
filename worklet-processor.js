class MyWorkletProcessor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.frequency = 440;
    this.n = 0;
}

process(inputs, outputs, parameters) {
    const output = outputs[0];
    const outputChannel = output[0];
    
    const fs = 44100;
    let output = 0;
    // 128 frames is the default output channel lenght for one buffer
    for (let i = 0; i < 128; i++) {
        //output = Math.sin((2 * Math.PI * this.frequency * this.n) / fs);
	outputChannel[i] = output;
        this.n++;
    }
    return true;
}
} // MyWorkletProcessor Class

registerProcessor('worklet-processor', MyWorkletProcessor);