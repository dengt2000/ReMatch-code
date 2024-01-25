import subprocess
import json
import sys
#from tqdm import tqdm
def run_executable_with_args(arg1, arg2):
    try:
        result = subprocess.run(['./match', str(arg1), str(arg2)],
                            capture_output=True, text=True,timeout=10)
        return result.stdout.strip().split()
    except subprocess.TimeoutExpired as e:
        raise e
def main():
    total_time = 0.0
    space_cost=0.0
    num=0
    json_file=sys.argv[1]
    wf=sys.argv[2]
    wfile=open(wf,'w')
    result={}
    with open(json_file, 'r') as file:
        for line in file:
            obj=json.loads(line.strip())
            arg1 = obj['Exp']
            arg2 = obj['w']
            #print(arg1)
            #print(arg2)
            try:
                output_args = run_executable_with_args(arg1, arg2)
                print(output_args[0])
                total_time += float(output_args[1])
                space_cost += float(output_args[2])
                num+=1
                if num%100==0:
                    print(num)
                result['Exp']=arg1
                result['w']=arg2
                s=json.dumps(result)
                wfile.write(s+'\n')
                #print(total_time)
                #print(space_cost)
            except subprocess.TimeoutExpired as te:
                #print(111)
                result['Exp']=arg1
                result['w']=arg2
                s=json.dumps(result)
                wfile.write(s+'\n')
                total_time+=100000
            except Exception as e:
                print(e)
                print(arg1)
                #print(arg2)
                #print(output_args)
    print("Total Time", total_time)
    print("Space Cost", space_cost)

if __name__ == "__main__":
    main()
