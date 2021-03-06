#ifndef _ANYSIGNAL_H_
#define _ANYSIGNAL_H_
#include "eagleeye/common/EagleeyeMacro.h"
#include "eagleeye/framework/pipeline/AnyUnit.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace eagleeye
{
class AnyNode;
class EAGLEEYE_API AnySignal:public AnyUnit
{
public:
	/**
	 *	@brief define some basic type
	 *	@note you must do these
	 */
	typedef AnySignal							Self;
	typedef AnyUnit								Superclass;

	AnySignal(const char* unit_name = "AnySignal", const char* signal_type="", const char* signal_target="");
	virtual ~AnySignal();

	/**
	 *	@brief Get class identity
	 */
	EAGLEEYE_CLASSIDENTITY(AnySignal);

	/**
	 *	@brief Set/Get pipeline time controlled by AnyNode
	 */
	unsigned long getPipelineTime();
	void setPipelineTime(unsigned long time);

	/**
	 *	@brief Link/Dislink AnyNonde 
	 */
	void linkAnyNode(AnyNode* node,int index);
	void dislinkAnyNode(AnyNode* node,int index);

	/**
	 *	@brief Copy info from signal
	 *	@note If you want to copy some specific info, you have to
	 *	overload this function. This function would be called by "passonNodeInfo"
	 *	of AnyNode, implicitly.
	 */
	virtual void copyInfo(AnySignal* sig){
		this->m_prepared_ok = this->m_prepared_ok&sig->isPreparedOK();
	};
	
	/**
	 * @brief copy content from signal
	 * 
	 * @param sig 
	 */
	virtual void copy(AnySignal* sig){};

	/**
	 * @brief clone signal
	 * 
	 * @return AnySignal* 
	 */
	virtual AnySignal* make(){return NULL;}

	/**
	 *	@brief Deliver the update flow
	 */
	virtual void updateUnitInfo();

	/**
	 *	@brief According to update time, notify the linked node
	 *	to process unit info dynamically.
	 */
	virtual void processUnitInfo();

	/**
	 *	@brief print this signal unit info
	 *	@note this function would be called by AnyNode object
	 */
	virtual void printUnit();

	/**
	 * @brief reset node backward
	 * 
	 */
	virtual void reset();

	/**
	 *	@brief one easy explained function, which notify itself
	 *	update.
	 */
	void signalHasBeenUpdate();

	/**
	 *	@brief get monitor pool of the whole pipeline
	 *	@note traverse the whole pipeline
	 */
	void getPipelineMonitors(std::map<std::string,std::vector<AnyMonitor*>>& pipeline_monitor_pool);

	/**
	 * @brief Get the Pipeline Inputs object
	 * 
	 * @param pipeline_inputs 
	 */
	void getPipelineInputs(std::map<std::string,AnyNode*>& pipeline_inputs);

	/**
	 *	@brief clear signal content
	 */
	virtual void makeempty(bool auto_empty=true){};

	/**
	 * @brief check signal content
	 * 
	 * @return true 
	 * @return false 
	 */
	virtual bool isempty(){return false;};

	/**
	 * @brief Set the Signal Type object
	 * 
	 * @param type
	 */
	void setSignalType(const char* type);

	/**
	 * @brief Set the Signal Type object
	 * 
	 * @param type 
	 */
	void setSignalType(SignalType type);

	/**
	 * @brief Get the Signal Type object
	 * 
	 * @return const char* 
	 */
	const char* getSignalType();

	/**
	 * @brief Set the Signal Target object
	 * 
	 * @param target 
	 */
	void setSignalTarget(const char* target);

	/**
	 * @brief Set the Signal Target object
	 * 
	 * @param target 
	 */
	void setSignalTarget(SignalTarget target);

	/**
	 * @brief Get the Signal Target object
	 * 
	 * @return const char* 
	 */
	const char* getSignalTarget();
	
	/**
	 * @brief Set the Signal Content object
	 * 
	 * @param data 
	 * @param data_size 
	 * @param data_dims 
	 */
	virtual void setSignalContent(void* data, const int* data_size, const int data_dims){};

	/**
	 * @brief Get the Signal Content object
	 * 
	 * @param data 
	 * @param data_size 
	 * @param data_dims 
	 */
	virtual void getSignalContent(void*& data, int* data_size, int& data_dims, int& data_type){};

	/**
	 * @brief whether content is prepared in the current signal 
	 * 
	 * @return true 
	 * @return false 
	 */
	virtual bool isPreparedOK(){return this->m_prepared_ok;}
	
	/**
	 * @brief increment out degree
	 * 
	 */
	void incrementOutDegree(){
		this->m_out_degree += 1;
	};
	/**
	 * @brief decrement out degree
	 * 
	 */
	void decrementOutDegree(){
		this->m_out_degree -= 1;
	};

	int getOutDegree(){return this->m_out_degree;}

	/**
	 * @brief feadback
	 * 
	 * @param node_state_map 
	 */
	void feadback(std::map<std::string, int>& node_state_map);

	/**
	 * @brief Get the Delay Time object
	 * 
	 * @return int 
	 */
	int getDelayTime(){return m_delay_time;}

	/**
	 * @brief Set the Delay Time object
	 * 
	 * @param delay_time 
	 */
	void setDelayTime(int delay_time){this->m_delay_time=delay_time;}
	/**
	 * @brief Get the Derive Type object
	 * 
	 * @return SignalCategory 
	 */
	virtual SignalCategory getSignalCategoryType(){return SIGNAL_CATEGORY_OTHER;}

	/**
	 * @brief Get the Signal Value Type object
	 * 
	 * @return int 
	 */
	virtual EagleeyeType getSignalValueType(){return EAGLEEYE_UNDEFINED;}

	/**
	 * @brief load/save pipeline configure
	 * 
	 * @param node_config 
	 */
	void loadConfigure(std::map<std::string, std::shared_ptr<char>> nodes_config);
	void saveConfigure(std::map<std::string, std::shared_ptr<char>>& nodes_config);

protected:
	std::string m_signal_type;
	std::string m_signal_target;
	bool m_prepared_ok;
	int m_delay_time;
	AnyNode* m_link_node;

private:
	AnySignal(const AnySignal&);
	void operator=(const AnySignal&);

	unsigned long m_pipeline_time;
	int m_link_index;
	int m_out_degree;

	std::map<SignalType,std::string> mSupportSignalType;
	std::map<SignalTarget,std::string> mSupportSignalTarget;
};
}

#endif
