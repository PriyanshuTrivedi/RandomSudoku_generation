/*Carry on my wayward son,
There'll be peace when you are done
Lay your weary head to rest
Don't you cry no more...*/

#include <bits/stdc++.h>
using namespace std;
#define cout std::cout
#define cin std::cin
using ll=int;

const ll n=9;
bool alreadyExists,multiSolutionExists;
vector<vector<pair<ll,ll>>>vv;
vector<vector<ll>>SudokuBoardThatGeneratesUniqueAnswer;
vector<vector<set<ll>>>possDigsForBoxij;
set<ll>canBePresent;
vector<ll>freq,rowNo,columnNo;
vector<pair<ll,ll>>boxIndx;
vector<vector<ll>>TempSudoku,FullyFilledSudoku;
vector<pair<ll,ll>>canTakeBox;

// **************************************************************************************************************************

pair<ll,ll> findRandomUnfilledBox(vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku)
{
    ll i,j;
    vv.assign(10,vector<pair<ll,ll>>());
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    if(tempSudoku[i][j]==-1)
    vv[possDigitsForBoxij[i][j].size()].push_back({i,j});
    i=1;
    while(i<10)
    {
        if(vv[i].size()!=0)
        return vv[i][rand()%vv[i].size()];
        i++;
    }
    return {-1,-1};
}
void updateBox(vector<ll>&deletedValuesInBox,vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku,ll i,ll j,ll chngedVal)
{
    tempSudoku[i][j]=chngedVal;
    for(auto &x:possDigitsForBoxij[i][j])
    deletedValuesInBox.push_back(x);
    possDigitsForBoxij[i][j].clear();
}
void updateVerticalLine(vector<ll>&row_noOfVerticalLinesAffected,vector<vector<set<ll>>>&possDigitsForBoxij,ll j,ll prohibitedval)
{
    ll k;
    for(k=0;k<n;k++)
    if(possDigitsForBoxij[k][j].find(prohibitedval)!=possDigitsForBoxij[k][j].end())
    {
        row_noOfVerticalLinesAffected.push_back(k);
        possDigitsForBoxij[k][j].erase(prohibitedval);
    }
}
void updateHorizontalLine(vector<ll>&column_noOfHorizontalLinesAffected,vector<vector<set<ll>>>&possDigitsForBoxij,ll i,ll prohibitedval)
{
    ll k;
    for(k=0;k<n;k++)
    if(possDigitsForBoxij[i][k].find(prohibitedval)!=possDigitsForBoxij[i][k].end())
    {
        column_noOfHorizontalLinesAffected.push_back(k);
        possDigitsForBoxij[i][k].erase(prohibitedval);
    }
}
void update3X3Box(vector<pair<ll,ll>>&boxesAffectedIn3X3Box,vector<vector<set<ll>>>&possDigitsForBoxij,ll i,ll j,ll prohibitedVal)
{
    ll k,l;
    for(k=i/3*3;k<i/3*3+3;k++)
    for(l=j/3*3;l<j/3*3+3;l++)
    if(possDigitsForBoxij[k][l].find(prohibitedVal)!=possDigitsForBoxij[k][l].end())
    {
        boxesAffectedIn3X3Box.push_back({k,l});
        possDigitsForBoxij[k][l].erase(prohibitedVal);
    }
}
void revUpdateBox(vector<vector<set<ll>>>&possDigitsForBoxij,vector<ll>&deletedValuesInBox,vector<vector<ll>>&tempSudoku,ll i,ll j)
{
    tempSudoku[i][j]=-1;
    for(auto &x:deletedValuesInBox)
    possDigitsForBoxij[i][j].insert(x);
    deletedValuesInBox.clear();
}
void revUpdateVerticalLine(vector<vector<set<ll>>>&possDigitsForBoxij,vector<ll>&row_noOfVerticalLinesAffected,ll j,ll toBeadded)
{
    for(auto &x:row_noOfVerticalLinesAffected)
    possDigitsForBoxij[x][j].insert(toBeadded);
    row_noOfVerticalLinesAffected.clear();
}
void revUpdateHorizontalLine(vector<vector<set<ll>>>&possDigitsForBoxij,vector<ll>&column_noOfHorizontalLinesAffected,ll i,ll toBeadded)
{
    for(auto &x:column_noOfHorizontalLinesAffected)
    possDigitsForBoxij[i][x].insert(toBeadded);
    column_noOfHorizontalLinesAffected.clear();
}
void revUpdate3X3Box(vector<vector<set<ll>>>&possDigitsForBoxij,vector<pair<ll,ll>>&boxesAffectedIn3X3Box,ll toBeAdded)
{
    for(auto &x:boxesAffectedIn3X3Box)
    possDigitsForBoxij[x.first][x.second].insert(toBeAdded);
    boxesAffectedIn3X3Box.clear();
}
void findNoSudokuBoard(vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku,ll i,ll j,ll tillNowSetBoxes)
{
    if(tillNowSetBoxes==n*n)
    {
        if(alreadyExists)
        multiSolutionExists=true;
        alreadyExists=true;
        return;
    }
    if(i==-1)
    return;
    vector<ll>deletedValuesInBox,row_noOfVerticalLinesAffected,column_noOfHorizontalLinesAffected;
    vector<pair<ll,ll>>boxesAffectedIn3X3Box;
    set<ll>temp=possDigitsForBoxij[i][j];
    for(auto x:temp)
    {
        updateBox(deletedValuesInBox,possDigitsForBoxij,tempSudoku,i,j,x);
        updateVerticalLine(row_noOfVerticalLinesAffected,possDigitsForBoxij,j,x);
        updateHorizontalLine(column_noOfHorizontalLinesAffected,possDigitsForBoxij,i,x);
        update3X3Box(boxesAffectedIn3X3Box,possDigitsForBoxij,i,j,x);
        pair<ll,ll>randomUnfilledBox=findRandomUnfilledBox(possDigitsForBoxij,tempSudoku);
        ll newi,newj;
        newi=randomUnfilledBox.first;
        newj=randomUnfilledBox.second;
        findNoSudokuBoard(possDigitsForBoxij,tempSudoku,newi,newj,tillNowSetBoxes+1);
        if(multiSolutionExists)
        return;
        revUpdateBox(possDigitsForBoxij,deletedValuesInBox,tempSudoku,i,j);
        revUpdateVerticalLine(possDigitsForBoxij,row_noOfVerticalLinesAffected,j,x);
        revUpdateHorizontalLine(possDigitsForBoxij,column_noOfHorizontalLinesAffected,i,x);
        revUpdate3X3Box(possDigitsForBoxij,boxesAffectedIn3X3Box,x);
    }
}

// **************************************************************************************************************************
void findCanBePresent(vector<vector<ll>>&tempSudoku,set<ll>&canBePresent,ll boxi,ll boxj)
{
    ll i,j;
    for(i=0;i<n;i++)
    {
        if(tempSudoku[boxi][i]!=-1)
        canBePresent.erase(tempSudoku[boxi][i]);
        if(tempSudoku[i][boxj]!=-1)
        canBePresent.erase(tempSudoku[i][boxj]);
    }
    for(i=boxi/3*3;i<boxi/3*3+3;i++)
    for(j=boxj/3*3;j<boxj/3*3+3;j++)
    if(tempSudoku[i][j]!=-1)
    canBePresent.erase(tempSudoku[i][j]);
}
bool checkIfUnique(vector<vector<ll>>&tempSudoku)
{
    TempSudoku=tempSudoku;
    possDigsForBoxij.assign(n,vector<set<ll>>(n,set<ll>()));
    ll i,j,k,tillNowSetBoxes,ii,jj;
    tillNowSetBoxes=n*n;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    if(TempSudoku[i][j]==-1)
    {
        ii=i,jj=j;
        for(k=1;k<=n;k++)
        canBePresent.insert(k);
        findCanBePresent(TempSudoku,canBePresent,i,j);
        for(auto &x:canBePresent)
        possDigsForBoxij[i][j].insert(x);
        canBePresent.clear();
        tillNowSetBoxes--;
    }
    multiSolutionExists=false;
    alreadyExists=false;
    findNoSudokuBoard(possDigsForBoxij,TempSudoku,ii,jj,tillNowSetBoxes);
    if(alreadyExists && !multiSolutionExists)
    return true;
    return false; 
}
bool makeUniqueSolvableBoard(vector<vector<ll>>&tempSudoku,ll totalNeededToBeRemoved)
{
    if(totalNeededToBeRemoved==0)
    {
        SudokuBoardThatGeneratesUniqueAnswer=tempSudoku;
        return true;
    }
    ll box1i,box1j,orVal1,x,sizeCanTakeBox;
    sizeCanTakeBox=canTakeBox.size();
    bool canErase;
    random_shuffle(canTakeBox.begin(),canTakeBox.end());
    for(x=0;x<sizeCanTakeBox;x++)
    {
        box1i=canTakeBox[x].first;
        box1j=canTakeBox[x].second;
        orVal1=tempSudoku[box1i][box1j];
        tempSudoku[box1i][box1j]=-1;
        canErase=checkIfUnique(tempSudoku);
        if(canErase)
        {
            swap(canTakeBox[x],canTakeBox[sizeCanTakeBox-1]);
            canTakeBox.pop_back();
            return makeUniqueSolvableBoard(tempSudoku,totalNeededToBeRemoved-1);
        }
        tempSudoku[box1i][box1j]=orVal1;
    }
    return false;
}


bool findSudokuBoard(vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku,ll i,ll j,ll tillNowSetBoxes)
{
    if(tillNowSetBoxes==n*n)
    {
        FullyFilledSudoku=tempSudoku;
        return true;
    }
    if(i==-1)
    return false;
    vector<ll>deletedValuesInBox,row_noOfVerticalLinesAffected,column_noOfHorizontalLinesAffected;
    vector<pair<ll,ll>>boxesAffectedIn3X3Box;
    set<ll>temp=possDigitsForBoxij[i][j];
    for(auto x:temp)
    {
        updateBox(deletedValuesInBox,possDigitsForBoxij,tempSudoku,i,j,x);
        updateVerticalLine(row_noOfVerticalLinesAffected,possDigitsForBoxij,j,x);
        updateHorizontalLine(column_noOfHorizontalLinesAffected,possDigitsForBoxij,i,x);
        update3X3Box(boxesAffectedIn3X3Box,possDigitsForBoxij,i,j,x);
        pair<ll,ll>randomUnfilledBox=findRandomUnfilledBox(possDigitsForBoxij,tempSudoku);
        ll newi,newj;
        newi=randomUnfilledBox.first;
        newj=randomUnfilledBox.second;
        if(findSudokuBoard(possDigitsForBoxij,tempSudoku,newi,newj,tillNowSetBoxes+1))
        return true;
        revUpdateBox(possDigitsForBoxij,deletedValuesInBox,tempSudoku,i,j);
        revUpdateVerticalLine(possDigitsForBoxij,row_noOfVerticalLinesAffected,j,x);
        revUpdateHorizontalLine(possDigitsForBoxij,column_noOfHorizontalLinesAffected,i,x);
        revUpdate3X3Box(possDigitsForBoxij,boxesAffectedIn3X3Box,x);
    }
    return false;
}
vector<ll> findPermOfN()
{
    vector<ll>v={1,2,3,4,5,6,7,8,9};
    ll i;
    for(i=0;i<n;i++)
    swap(v[i],v[rand()%n]);
    return v;
}
void fill3X3GivenTopLeftBox(vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku,ll sti,ll stj)
{
    ll i,j,k,l;
    vector<ll>perm=findPermOfN();
    k=0;
    for(i=sti;i<sti+3;i++)
    for(j=stj;j<stj+3;j++)
    {
        tempSudoku[i][j]=perm[k];
        possDigitsForBoxij[i][j].clear();
        k++;
    }
    for(j=0;j<n;j++)
    {
        for(k=0;k<3;k++)
        for(l=0;l<3;l++)
        possDigitsForBoxij[sti+k][j].erase(perm[k*3+l]);
        for(k=0;k<3;k++)
        {
            possDigitsForBoxij[j][stj+k].erase(perm[0+k]);
            possDigitsForBoxij[j][stj+k].erase(perm[3+k]);
            possDigitsForBoxij[j][stj+k].erase(perm[6+k]);
        }
    }
}
void initialFill(vector<vector<set<ll>>>&possDigitsForBoxij,vector<vector<ll>>&tempSudoku)
{
    fill3X3GivenTopLeftBox(possDigitsForBoxij,tempSudoku,0,0);
    fill3X3GivenTopLeftBox(possDigitsForBoxij,tempSudoku,3,3);
    fill3X3GivenTopLeftBox(possDigitsForBoxij,tempSudoku,6,6);
}

void createRandomSudoku()
{
    string ans="";
    ll i,j,k,tillNowSetBoxes;
    vector<vector<set<ll>>>possDigitsForBoxij(n,vector<set<ll>>(n,set<ll>()));
    vector<vector<ll>>tempSudoku(n,vector<ll>(n,-1));
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    for(k=1;k<=n;k++)
    possDigitsForBoxij[i][j].insert(k);
    initialFill(possDigitsForBoxij,tempSudoku);
    i=0,j=3;
    findSudokuBoard(possDigitsForBoxij,tempSudoku,i,j,27);
}

void print(vector<vector<ll>>&temp_name_for_printing_sudoku)
{
    for(auto &x:temp_name_for_printing_sudoku)
    for(auto &y:x)
    {
        if(y==-1)
        cout<<'.';
        else
        cout<<y;
    }
}
int main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    srand(time(0));
    vector<vector<ll>>Sudoku(n,vector<ll>(n,-1));
    ll i,j,k,blanks;
    createRandomSudoku();
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        Sudoku[i][j]=FullyFilledSudoku[i][j];
        canTakeBox.push_back({i,j});
    }
    makeUniqueSolvableBoard(Sudoku,53);
    blanks=0;
    print(FullyFilledSudoku);
    cout<<",";
    print(SudokuBoardThatGeneratesUniqueAnswer);
    return 0;
}