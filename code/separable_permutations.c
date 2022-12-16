#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void print_array(size_t length, int *arr)
{
    for(size_t i=0; i<length; i++)
        printf("%d ",arr[i]);
    printf("\n");
}

// checks if arr holds (all) the numbers [0,n) in some order
bool is_permutation(size_t n, int *arr)
{
    size_t i;
    bool result = true;
    bool *temp = malloc(sizeof(bool)*n);
    for(i=0; i<n; i++)
        temp[i]=false;
    for(i=0; i<n; i++)
    {
        if(arr[i] < 0 || n-1 < arr[i])
        {
            result = false;
            goto finish;
        }
        temp[arr[i]] = true;
    }
    for(i=0; i<n; i++)
    {
        if(temp[i]==false)
        {
            result = false;
            goto finish;
        }
    }

    finish:
    free(temp);
    return result;
}

int* new_array(size_t n)
{
    int *x = malloc(sizeof(int)*n);
    assert(x != NULL);
    return x;
}

bool is_separable_permutation(size_t n, int *arr)
{
    if(n<4)
        return true;
    assert(is_permutation(n,arr));
    //printf("=========================================================\n");
    //printf("checking separability of permutation : ");
    //print_array(n, arr);

    int *dom_to_img = new_array(n);
    int *dom_left = new_array(n);
    int *dom_right = new_array(n);
    int *img_right = new_array(n);
    int *dom_parent = new_array(n);
    int *img_parent = new_array(n);
    for(size_t i=0; i<n; i++)
    {
        dom_to_img[i] = arr[i];
        dom_left[i] = i-1;
        dom_right[i] = i+1;
        img_right[i] = i+1;
        dom_parent[i] = i;
        img_parent[i] = i;
    }
    int merge_count = 0;

    int current = 0;
    int next, img_of_current, img_of_next;
    //int count = 0;
    while(true)
    {
        assert(current < n);
        assert(dom_parent[current] == current);
        next = dom_right[current];
        assert(next > current);
        
        if(next >= n)
        {
            assert(next == n);
            free(dom_to_img);
            free(dom_left);
            free(dom_right);
            free(img_right);
            free(dom_parent);
            free(img_parent);
            return merge_count == n-1;
        }

        assert(dom_parent[next] == next);
        assert(dom_left[next] == current);

        img_of_current = dom_to_img[current];
        img_of_next = dom_to_img[next];

        assert(img_parent[img_of_current] == img_of_current);
        assert(img_parent[img_of_next] == img_of_next);

        if(img_right[img_of_current] == img_of_next)
        {
            //printf("merge :: domain: %d into %d, image: %d into %d\n", next, current, img_of_next, img_of_current);
            merge_count++;
            
            dom_parent[next] = current;
            dom_right[current] = dom_right[next];
            if(dom_right[current] < n)
                dom_left[dom_right[current]] = current;

            if(-1 < dom_left[current])
                current = dom_left[current];

            img_parent[img_of_next] = img_of_current;
            img_right[img_of_current] = img_right[img_of_next];
            continue;
        }

        if(img_right[img_of_next] == img_of_current)
        {
            //printf("merge :: domain: %d into %d, image: %d into %d\n", next, current, img_of_current, img_of_next);

            merge_count++;
            
            dom_parent[next] = current;
            dom_right[current] = dom_right[next];
            dom_to_img[current] = img_of_next;
            if(dom_right[current] < n)
                dom_left[dom_right[current]] = current;

            if(-1 < dom_left[current])
                current = dom_left[current];

            img_parent[img_of_current] = img_of_next;
            img_right[img_of_next] = img_right[img_of_current];
            continue;
        }
        current = next;
    }

}


void swap(size_t ind1, size_t ind2, int *arr)
{
    int temp = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = temp;
}

int tot = 0;

void print_all_separable_permutations(size_t n, size_t length, int *arr)
{
    assert(1<=n && n<=length);
    if(n==1)
    {   
        assert(is_permutation(length,arr));
        if(is_separable_permutation(length,arr))
        {
            tot++;
            //("%d tot\n",tot);
            print_array(length, arr);
        }
        return;
    }
    for(size_t i=0; i<n; i++)
    {
        swap(i,n-1,arr);
        print_all_separable_permutations(n-1,length,arr);
        swap(i,n-1,arr);
    }
}


//test
int main()
{
    int arr[10] = {0,1,2,3,4,5,6,7,8,9};//,3};
    //bool x = is_separable_permutation(4,arr);
    //if(x)
    //    printf("is separable\n");
    //else
    //    printf("not separable\n");
    print_all_separable_permutations(10,10,arr);
    printf("total number of separable permutations = %d\n",tot);
    return 0;
}
