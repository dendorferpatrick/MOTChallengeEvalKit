import sys, os
sys.path.append(os.path.abspath(os.getcwd()))
import math
from collections import defaultdict
from DET_metrics import DETMetrics
from Evaluator import Evaluator, run_metrics
import multiprocessing as mp


class DET_evaluator(Evaluator):
	def __init__(self):
		self.type = "DET"
	def eval(self):

		arguments = []
		
		for seq, res, gt in zip(self.sequences, self.tsfiles, self.gtfiles):

			arguments.append({"metricObject": DETMetrics(seq), "args" : {
			"gtDataDir":  os.path.join( self.datadir,seq),
			"sequence": str(seq) ,
			"pred_file":res,
			"gt_file": gt,
			"benchmark_name": self.benchmark_name}})



		if self.MULTIPROCESSING:

			p = mp.Pool(self.NR_CORES)

			print("Evaluating on {} cpu cores".format(self.NR_CORES))
			processes = [p.apply_async(run_metrics, kwds=inp) for inp in arguments]
			self.results = [p.get() for p in processes]
			p.close()
			p.join()

		else:
			print("Evaluating sequentially")
			self.results = [run_metrics(**inp) for inp in arguments]
		self.failed = False
		self.Overall_Results = DETMetrics("OVERALL")


if __name__ == "__main__":
	eval = DET_evaluator()

	benchmark_name = "MOT17Det"
	gt_dir = "data/MOT17Det"
	res_dir = "res/MOT17Detres"
	seq_file = "c9-train.txt"
	eval_mode = "train"
	eval.run(
		benchmark_name = benchmark_name,
		gt_dir = gt_dir,
		res_dir = res_dir,
		eval_mode = eval_mode)
