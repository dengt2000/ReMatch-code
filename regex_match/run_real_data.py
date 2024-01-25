import subprocess
import json
import sys
#from tqdm import tqdm
def run_executable_with_args(arg1, arg2):
    try:
        result = subprocess.run(['./match', str(arg1), str(arg2)],
                            capture_output=True, text=True,timeout=100)
        return result.stdout.strip().split()
    except subprocess.TimeoutExpired as e:
        raise e
def main():
    total_time = 0.0
    space_cost=0.0
    error_count=0
    num=0
    json_file=sys.argv[1]
    result={}
    log_file=open('cleanlog.txt','w')
    with open(json_file, 'r') as file:
        for line in file:
            obj=json.loads(line.strip())
            arg1 = obj['Exp']
            arg2 = obj['w']
            #print(arg1)
            #print(arg2)
            try:
                output_args = run_executable_with_args(arg1, arg2)
                #print(output_args[0])
                total_time += float(output_args[3])
                space_cost += float(output_args[4])
                json.dump({'exp':arg1,'time':output_args[3]},log_file)
                log_file.write('\n')
                num+=1
                if num%100==0:
                    print(num)
                #result['Exp']=arg1
                #result['w']=arg2
                #s=json.dumps(result)
                #wfile.write(s+'\n')
                #print(total_time)
                #print(space_cost)
            except subprocess.TimeoutExpired as te:
                total_time+=100000
                json.dump({'exp':arg1,'time':100000},log_file)
                log_file.write('\n')
                print(arg1)
                print(te)
            except Exception as e:
                error_count+=1
                json.dump({'exp':arg1,'time':-1},log_file)
                log_file.write('\n')
                print(e)
                print(arg1)
                #print(arg2)
                #print(output_args)
    print("Total Time", total_time)
    print("Space Cost", space_cost)
    print("Error Count", error_count)

if __name__ == "__main__":
    main()
