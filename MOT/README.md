# MOT - Multi Object Tracking
![MOT_PIC](https://motchallenge.net/sequenceVideos/MOT17-04-SDP-gt.jpg)
```diff
- IMPORTANT!
- The MOT evaluation code is not any longer maintained. 
```
Please visit the [new official python evaluation code](https://github.com/JonathonLuiten/TrackEval/blob/master/docs/MOTChallenge-Official/Readme.md). 


## Requirements
* Python 3.6.9
* MATLAB (> R2014b) 
* C/C++ compiler
* matlab python engine (https://www.mathworks.com/help/matlab/matlab_external/get-started-with-matlab-engine-for-python.html)
* install [requirements.txt](requirements.txt)
* Note: A compatible Python implementation is available at: https://github.com/cheind/py-motmetrics
## Usage

1) Compile the matlab evaluation code
```
matlab matlab_devkit/compile.m
```
2) Run
```
python MOT/evalMOT.py
```


## Evaluation
To run the evaluation for your method please adjust the file ```MOT/evalMOT.py``` using the following arguments:

```benchmark_name```: Name of the benchmark, e.g. MOT17  
```gt_dir```: Directory containing ground truth files in ```<gt_dir>/<sequence>/gt/gt.txt```    
```res_dir```: The folder containing the tracking results. Each one should be saved in a separate .txt file with the name of the respective sequence (see ./res/data)    
```save_pkl```: path to output directory for final results (pickle)  (default: False)  
```eval_mode```: Mode of evaluation out of ```["train", "test", "all"]``` (default : "train")

```
eval.run(
    benchmark_name = benchmark_name,
    gt_dir = gt_dir,
    res_dir = res_dir,
    eval_mode = eval_mode
    )
```
## Visualization
To visualize your results or the annotations run
<code>
python MOT/MOTVisualization.py
</code>

Inside the script adjust the following values for the ```MOTVisualizer``` class:

```seqName```: Name of the sequence  
```FilePath```: Data file  
```image_dir```: Directory containing images  
```mode```: Video mode. Options: ```None``` for method results, ```raw``` for data video only, and ```gt``` for annotations  
```output_dir```: Directory for created video and thumbnail images  

Additionally, adjust the following values for the ```generateVideo``` function:

```displayTime```: If true, display frame number (default false)  
```displayName```: Name of the method  
```showOccluder```: If true, show occluder of gt data  
```fps```: Frame rate  

```
visualizer = MOTVisualizer(seqName, FilePath, image_dir, mode, output_dir )
visualizer.generateVideo(displayTime, displayName, showOccluder, fps  )
```

## Data Format
<p>
The file format should be the same as the ground truth file, 
which is a CSV text-file containing one object instance per line.
Each line must contain 9 values:
</p>

</p>
<code>
&lt;frame&gt;,
&lt;id&gt;,
&lt;bb_left&gt;,
&lt;bb_top&gt;,
&lt;bb_width&gt;,
&lt;bb_height&gt;,
&lt;conf&gt;,
&lt;class&gt;,
&lt;visibility&gt;
</code>
</p>

Bounding box coordiantes and sizes are rounded to the nearest integer. Visibility varies between 0 (object is completely occluded) and 1 (object is completely visible). Only pedestrians are ultimately scored, and predicted bounding boxes that overlap by > 50% with a ground-truth object of a different class are removed so as not to penalize detectors for these false positives. All ground-truth confidence values for pedestrians are 1, and all other class confidences are 0.Classes indices  are as follows:
- Pedestrian 1
- Person on vehicle 2
- Car 3
- Bicycle 4
- Motorbike 5
- Non motorized vehicle 6
- Static person 7
- Distractor 8
- Occluder 9
- Occluder on the ground 10
- Occluder full 11
- Reflection 12

All frame numbers, target IDs and bounding boxes are 1-based. Here is an example:
<pre>
1, 3, 794.27, 247.59, 71.245, 174.88, 1, 1, 1
1, 6, 1648.1, 119.61, 66.504, 163.24, 1, 1, 0.956
1, 8, 875.49, 399.98, 95.303, 233.93, 1, 1, 0.874
1, 9, 875.49, 399.98, 95.303, 233.93, 0, 7, 0.743
...
</pre>

 
## Evaluating on your own Data
The repository also allows you to include your own datasets and evaluate your method on your own challenge ```<YourChallenge>```.  To do so, follow these two steps:  
***1. Ground truth data preparation***  
Prepare your sequences in directory ```~/data/<YourChallenge>``` following this structure:

```
.
|—— <SeqName01>
	|—— gt
		|—— gt.txt
	|—— det
		|—— det.txt
	|—— img1
		|—— 000001.jpg
		|—— 000002.jpg
		|—— ….
|—— <SeqName02>
	|—— ……
|—— <SeqName03>
	|—— …...
```
If you have different image sources for the same sequence or do not provide public detections you can adjust the structure accordingly.  
***2. Sequence file***  
Create text files containing the sequence names; ```<YourChallenge>-train.txt```, ```<YourChallenge>-test.txt```,  ```<YourChallenge>-test.txt``` inside ```~/seqmaps```, e.g.:
```<YourChallenge>-all.txt```
```
name
<seqName1> 
<seqName2>
<seqName3>
```

```<YourChallenge>-train.txt```
```
name
<seqName1> 
<seqName2>
```

```<YourChallenge>-test.txt```
```
name
<seqName3>
```

To run the evaluation for your method adjust the file ```MOT/evalMOT.py``` and set ```benchmark_name = <YourChallenge>``` and ```eval_mode```: Mode of evaluation out of ```["train", "test", "all"]``` (default : "train")

## Citation
If you work with the code and the benchmark, please cite:

***MOTChallenge***
```
@article{dendorfer2020motchallenge,
  title={MOTChallenge: A Benchmark for Single-camera Multiple Target Tracking},
  author={Dendorfer, Patrick and Osep, Aljosa and Milan, Anton and Schindler, Konrad and Cremers, Daniel and Reid, Ian and Roth, Stefan and Leal-Taix{\'e}, Laura},
  journal={International Journal of Computer Vision},
  pages={1--37},
  year={2020},
  publisher={Springer}
}
```
***MOT 15***
```
@article{MOTChallenge2015,
	title = {{MOTC}hallenge 2015: {T}owards a Benchmark for Multi-Target Tracking},
	shorttitle = {MOTChallenge 2015},
	url = {http://arxiv.org/abs/1504.01942},
	journal = {arXiv:1504.01942 [cs]},
	author = {Leal-Taix\'{e}, L. and Milan, A. and Reid, I. and Roth, S. and Schindler, K.},
	month = apr,
	year = {2015},
	note = {arXiv: 1504.01942},
	keywords = {Computer Science - Computer Vision and Pattern Recognition}
}
```
***MOT 16, MOT 17***
```
@article{MOT16,
	title = {{MOT}16: {A} Benchmark for Multi-Object Tracking},
	shorttitle = {MOT16},
	url = {http://arxiv.org/abs/1603.00831},
	journal = {arXiv:1603.00831 [cs]},
	author = {Milan, A. and Leal-Taix\'{e}, L. and Reid, I. and Roth, S. and Schindler, K.},
	month = mar,
	year = {2016},
	note = {arXiv: 1603.00831},
	keywords = {Computer Science - Computer Vision and Pattern Recognition}
}
```
***MOT 20***
```
@article{MOTChallenge20,
    title={MOT20: A benchmark for multi object tracking in crowded scenes},
    shorttitle = {MOT20},
	url = {http://arxiv.org/abs/2003.09003},
	journal = {arXiv:2003.09003[cs]},
	author = {Dendorfer, P. and Rezatofighi, H. and Milan, A. and Shi, J. and Cremers, D. and Reid, I. and Roth, S. and Schindler, K. and Leal-Taix\'{e}, L. },
	month = mar,
	year = {2020},
	note = {arXiv: 2003.09003},
	keywords = {Computer Science - Computer Vision and Pattern Recognition}
}
```

## Contact
If you find a problem in the code, please open an issue.

For general questions, please contact Patrick Dendorfer (patrick.dendorfer@tum.de) or Aljosa Osep (aljosa.osep@tum.de)
