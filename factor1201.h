#ifndef FACTOR0_H
#define FACTOR0_H

using namespace std;
class Factor
{
    public:
        Factor():n(0),addedn(0),cnt1(0),cnt2(0),cnt3(0),cnt4(0),cnt(0) {};
        virtual ~Factor(){};
        vector<int> f(int& x){  //
            vector<int> pnlist;
            for (int i=2;i<=maxfanout;i++){
                while (x%i==0){
                    x=x/i;
                    pnlist.push_back(i);
                }
            }
            return pnlist;
        }
        void QF(int m){
            int t=m;
            n=m;

            if (t==1)
                primenumlist.push_back(1);
            else{
                primenumlist=f(t);
                while(t>maxfanout){
                    primenumlist.clear();
                    m+=1;
                    addedn=t=m;
                    primenumlist=f(t);
                }
            }
            for(unsigned int i=0;i<primenumlist.size();i++){
                if(primenumlist[i]==2)
                    cnt1+=1;
                if(primenumlist[i]==3)
                    cnt2+=1;
                if(primenumlist[i]==5)
                    cnt3+=1;
                if(primenumlist[i]==7)
                    cnt4+=1;
            }
            cnt=cnt1+cnt2+cnt3+cnt4;
//            cout<<"QF : cnt1 = "<<cnt1<<" cnt2 = "<<cnt2<<" cnt3 = "<<cnt3<<" cnt4 = "<<cnt4<<endl;
        }

        pair<int, int> NQF(int m){
            QF(m);
            if(m == 0 || m == 1)
                return make_pair(m, 0);
            else
                return make_pair(max(n,addedn),cnt);
        }

        vector<int> element (vector<int>& node_number){
            vector<int> level;
//            cout << "cnt1: " << cnt1 << " cnt2: " << cnt2 << " cnt3: " << cnt3 << " cnt4: " << cnt4 << " node: " << addedn << endl;
            for(int i=0; i<cnt1; i++)
            {
                node_number.push_back(max(n,addedn) / pow(2,i));
                level.push_back(2);
//                cout << "node number for level " << i << " = " << node_number[i] <<endl;
            }
            for(int j=0; j<cnt2; j++)
            {
                node_number.push_back(max(n,addedn) / (pow(2,cnt1)*pow(3,j)));
                level.push_back(3);
//                cout << "node number for level " << j+cnt1 << " = " << node_number[j+cnt1] <<endl;
            }
            for(int j=0; j<cnt3; j++)
            {
                node_number.push_back(max(n,addedn) / (pow(2,cnt1)*pow(3,cnt2)*pow(5,j)));
                level.push_back(5);
//                cout << "node number for level " << j+cnt1+cnt2 << " = " << node_number[j+cnt1+cnt2] <<endl;
            }
            for(int j=0; j<cnt4; j++)
            {
                node_number.push_back(max(n,addedn) / (pow(2,cnt1)*pow(3,cnt2)*pow(5,cnt3)*pow(7,j)));
                level.push_back(7);
//                cout << "node number for level " << j+cnt1+cnt2+cnt3 << " = " << node_number[j+cnt1+cnt2+cnt3] <<endl;
            }

       //     level.pop_back();
//            for(int c : level)
//                cout << "level method : " << c <<endl;
            return level;
        }
        int getn(){
			return n;
		}
		int getaddedn(){
			return addedn;
		}
		vector<int> getprimenumlist(){
            return primenumlist;
		}

    private:
        int n, addedn,cnt1,cnt2,cnt3,cnt4,cnt,maxfanout=7;
        vector<int> primenumlist;
};

#endif // FACTOR0_H
